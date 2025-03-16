/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2023 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Battery Buddy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#include "battery.h"

Battery::Battery(Settings* newSettings, Logger* newLogger, QCoreApplication *app, QObject *parent)
    : BatteryBase(newLogger, parent)
{
    settings = newSettings;

    if(maxCurrentFile) {
        if(maxCurrentFile->open(QIODevice::WriteOnly)) {
            maxCurrentFile->close();
            if(maxCurrentFile->open(QIODevice::ReadOnly)) {

                // Read and store the default max current
                maxSupportedCurrent = maxCurrentFile->readLine().trimmed().toInt();
                logL(QString("Maximum supported charge current: %1mA").arg(maxSupportedCurrent / 1000));
                maxCurrentFile->close();
                settings->setMaxSupportedCurrent(maxSupportedCurrent);

                // Read and maybe set the user-set max current
                maxChargeCurrent = settings->getMaxChargeCurrent();
                if(maxChargeCurrent != maxSupportedCurrent) {
                    setMaxChargeCurrent(maxChargeCurrent);
                }
            }
        }
        else {
            logL("Max charge current file is not writable - feature disabled");
            delete maxCurrentFile;
            maxCurrentFile = nullptr;
        }
    }

    if(controlFile) {
        if(controlFile->open(QIODevice::WriteOnly)) {
            // Flip the charging control bits if necessary
            if(controlFile->fileName().contains("enable")) {
                enableChargingValue = 1 - enableChargingValue;
                disableChargingValue = 1 - disableChargingValue;
            }
            controlFile->close();
            logL(QString("Charger control is available"));
        }
        else {
            logL("Charger control file is not writable - feature disabled");
            delete controlFile;
            controlFile = nullptr;
        }
    }

    chargeNotification = new MyNotification(this);
    healthNotification = new MyNotification(this);

    connect(settings, SIGNAL(resetTimers()), this, SLOT(resetTimers()));
    connect(settings, SIGNAL(setMaxChargeCurrent(int)), this, SLOT(setMaxChargeCurrent(int)));

    updateTimer = new BackgroundActivity(app);
    highNotifyTimer = new BackgroundActivity(app);
    lowNotifyTimer = new BackgroundActivity(app);
    healthNotifyTimer = new BackgroundActivity(app);

    connect(updateTimer, SIGNAL(running()), this, SLOT(updateData()));
    connect(highNotifyTimer, SIGNAL(running()), this, SLOT(showHighNotification()));
    connect(lowNotifyTimer, SIGNAL(running()), this, SLOT(showLowNotification()));
    connect(healthNotifyTimer, SIGNAL(running()), this, SLOT(showHealthNotification()));

    connect(updateTimer, SIGNAL(running()), updateTimer, SLOT(wait()));
    connect(highNotifyTimer, SIGNAL(running()), highNotifyTimer, SLOT(wait()));
    connect(lowNotifyTimer, SIGNAL(running()), lowNotifyTimer, SLOT(wait()));
    connect(healthNotifyTimer, SIGNAL(running()), healthNotifyTimer, SLOT(wait()));

    connect(this, SIGNAL(_chargeChanged(int)), this, SIGNAL(chargeChanged(int)));
    connect(this, SIGNAL(_currentChanged(int)), this, SIGNAL(currentChanged(int)));
    connect(this, SIGNAL(_stateChanged(QString)), this, SIGNAL(stateChanged(QString)));
    connect(this, SIGNAL(_chargingEnabledChanged(bool)), this, SIGNAL(chargingEnabledChanged(bool)));
    connect(this, SIGNAL(_chargerConnectedChanged(bool)), this, SIGNAL(chargerConnectedChanged(bool)));
    connect(this, SIGNAL(_acConnectedChanged(bool)), this, SIGNAL(acConnectedChanged(bool)));
    connect(this, SIGNAL(_healthChanged(QString)), this, SIGNAL(healthChanged(QString)));
    connect(this, SIGNAL(_temperatureChanged(int)), this, SIGNAL(temperatureChanged(int)));

    connect(this, SIGNAL(healthChanged(QString)), this, SLOT(healthHandler(QString)));
    connect(this, SIGNAL(stateChanged(QString)), this, SLOT(stateHandler(QString)));

    updateData();

    updateTimer->setWakeupFrequency(BackgroundActivity::ThirtySeconds);
    updateTimer->wait();

    // If updateData() didn't start the timers
    // aka. "charging" status didn't change
    // (or if both times are disabled, actually)
    //  manually trigger the timer startup.
    if(!highNotifyTimer->isWaiting() && !lowNotifyTimer->isWaiting() && !healthNotifyTimer->isWaiting()) {
        resetTimers();
    }
}

Battery::~Battery() {
    setMaxChargeCurrent(maxChargeCurrent);

    updateTimer->stop();
    highNotifyTimer->stop();
    lowNotifyTimer->stop();
    healthNotifyTimer->stop();

    delete updateTimer;
    delete highNotifyTimer;
    delete lowNotifyTimer;
    delete healthNotifyTimer;
}

void Battery::updateData()
{
    updateBaseData();

    if(controlFile && settings->getLimitEnabled()) {
        if(chargingEnabled && charge >= settings->getHighLimit()) {
            logM("Disabling charging...");
            setChargingEnabled(false);
        }
        else if(!chargingEnabled && charge <= settings->getLowLimit()) {
            logM("Enabling charging...");
            setChargingEnabled(true);
        }
    }
}

void Battery::healthHandler(QString newHealth) {
    Q_UNUSED(newHealth);
    resetTimers();
}

void Battery::stateHandler(QString newState) {
    Q_UNUSED(newState);
    resetTimers();
}

void Battery::resetTimers() {
    highNotifyTimer->stop();
    lowNotifyTimer->stop();
    healthNotifyTimer->stop();


    if(settings->getHighNotificationsInterval() > 0) {
        highNotifyTimer->setWakeupFrequency(frequencies[settings->getHighNotificationsInterval()]);
        logH(QString("High notifications frequency %1 => %2 seconds")
             .arg(settings->getHighNotificationsInterval())
             .arg(static_cast<int>(frequencies[settings->getHighNotificationsInterval()])));
        logH("Starting high battery timer");
        highNotifyTimer->wait();
        showHighNotification();
    }
    else {
        logH("High battery timer not started");
    }

    if(settings->getLowNotificationsInterval() > 0) {
        lowNotifyTimer->setWakeupFrequency(frequencies[settings->getLowNotificationsInterval()]);
        logH(QString("Low notifications frequency %1 => %2 seconds")
             .arg(settings->getLowNotificationsInterval())
             .arg(static_cast<int>(frequencies[settings->getLowNotificationsInterval()])));
        logH("Start low battery timer");
        lowNotifyTimer->wait();
        showLowNotification();
    }
    else {
        logH("Low battery timer not started");
    }

    if(settings->getHealthNotificationsInterval() > 0) {
        healthNotifyTimer->setWakeupFrequency(frequencies[settings->getHealthNotificationsInterval()]);
        logH(QString("Health notifications frequency %1 => %2 seconds")
             .arg(settings->getHealthNotificationsInterval())
             .arg(static_cast<int>(frequencies[settings->getHealthNotificationsInterval()])));
        logH("Start health timer");
        healthNotifyTimer->wait();
        showHealthNotification();
    }
    else {
        logH("Health timer not started");
    }
}

void Battery::showHighNotification() {
    if(settings->getHighNotificationsInterval() > 0 && (charge >= settings->getHighAlert() && state != "discharging")
            && !(charge == 100 && state == "idle")) {
        logH(QString("Notification: %1").arg(settings->getNotificationTitle().arg(charge)));
        chargeNotification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationHighText(), settings->getHighAlertFile());
        unclosedChargeNotification = true;
        if(settings->getHighNotificationsInterval() == 50) {
            logH("Stop high battery timer");
            highNotifyTimer->stop();
        }
    }
    else if(unclosedChargeNotification && charge > settings->getLowAlert()) {
        logH("Close high battery notification");
        chargeNotification->close();
        unclosedChargeNotification = true;
    }
}

void Battery::showLowNotification() {
    if(settings->getLowNotificationsInterval() > 0 && charge <= settings->getLowAlert() && state != "charging") {
        logH(QString("Notification: %1").arg(settings->getNotificationTitle().arg(charge)));
        chargeNotification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationLowText(), settings->getLowAlertFile());
        unclosedChargeNotification = true;
        if(settings->getLowNotificationsInterval() == 50) {
            logH("Stop low battery timer");
            lowNotifyTimer->stop();
        }
    }
    else if(unclosedChargeNotification && charge < settings->getHighAlert()) {
        logH("Close low battery notification");
        chargeNotification->close();
        unclosedChargeNotification = true;
    }
}

void Battery::showHealthNotification() {
    // set up alert categories
    // TODO: manage this more globally, use better data types(?), align with QML/Settings part
    static const QMap<QString, int> HealthThresh {
        { "ok"   , 0},
        { "warn" , 1},
        { "crit" , 2},
    };
    // map string values from sysfs file to alert category
    static const QMap<QString, int> HealthState {
        { "unknown"  , HealthThresh["ok"] },
        { "good"     , HealthThresh["ok"] },
        { "warm"     , HealthThresh["warn"] },
        { "cool"     , HealthThresh["warn"] },
        { "overheat" , HealthThresh["crit"] },
        { "cold"     , HealthThresh["crit"] }
    };
    if(settings->getHealthNotificationsInterval() > 0 && temperature != 0x7FFFFFFF && ( HealthState[health] != HealthThresh["ok"] && HealthState[health] >= settings->getHealthAlert() ) ) {
        QString displayTemp = QString::number(temperature / 10.0);
        if (QLocale().measurementSystem() == QLocale::ImperialUSSystem)
            displayTemp = QString::number((temperature / 10) * 1.8 + 32) + " F";

        QString titleArgs;
        titleArgs = health + " (" + displayTemp + "), " + state; // might show other things in the future

        // show different test depending on severity
        QString notificationText = "";
        if (HealthState[health] == HealthThresh["warn"]) {
            notificationText =  settings->getNotificationHealthWarnText();
        } else if (HealthState[health] == HealthThresh["crit"]) {
            notificationText =  settings->getNotificationHealthCritText();
        }
        logH(QString("Notification: %1").arg(settings->getNotificationHealthTitle().arg(titleArgs)));
        healthNotification->send(settings->getNotificationHealthTitle().arg(titleArgs), notificationText, settings->getHealthAlertFile());
        unclosedHealthNotification = true;
        if(settings->getHealthNotificationsInterval() == 50) {
            logH("Stop health timer");
            healthNotifyTimer->stop();
        }
    }
    else if(unclosedHealthNotification && (HealthState[health] == HealthThresh["ok"] || HealthState[health] < settings->getHealthAlert())) {
        logH("Close health notification");
        healthNotification->close();
    }
}

bool Battery::setChargingEnabled(const bool isEnabled) {
    bool success = false;
    if(controlFile) {
        if(controlFile->open(QIODevice::WriteOnly)) {
            if(controlFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
                chargingEnabled = isEnabled;
                success = true;

                if(isEnabled) {
                    logM("Charging resumed");
                }
                else {
                    logM("Charging paused");
                }
            }
            else {
                logL("Could not write new charger state");
            }
            controlFile->close();
        }
        else {
            logL("Could not open charger control file");
        }
    }
    return success;
}

void Battery::setMaxChargeCurrent(int newCurrent) {
    if(maxCurrentFile) {
        logM(QString("Max charging current: %1mA").arg(newCurrent / 1000));
        if(newCurrent > maxChargeCurrent) {
            newCurrent = maxChargeCurrent;
        }
        if(maxCurrentFile->open(QIODevice::WriteOnly)) {
            QString data = QString("%1").arg(newCurrent);
            if(!maxCurrentFile->write(data.toLocal8Bit())) {
                logM("Could not write to max charging current file");
            }
        }
        else {
            logM("Could not open max charging current file");
        }
        maxCurrentFile->close();
    }
}

void Battery::shutdown() {
    logM("Shutting down...");
    chargeNotification->close();
    blockSignals(true);
    if(updateTimer) {
        updateTimer->stop();
        logH("Update timer stopped");
    }
    if(highNotifyTimer) {
        highNotifyTimer->stop();
        logH("High battery notification stopped");
    }
    if(lowNotifyTimer) {
        lowNotifyTimer->stop();
        logH("Low battery notification stopped");
    }
    if(healthNotifyTimer) {
        healthNotifyTimer->stop();
        logH("Health notification stopped");
    }
    if(!setChargingEnabled(true) && !QSysInfo::machineHostName().contains("SailfishEmul")) {
        logL("ERROR! Could not restore charger status! Your device "
             "may not charge until reboot! If that doesn't help, "
             "uninstall Battery Buddy and reboot your device.");
    }
}
