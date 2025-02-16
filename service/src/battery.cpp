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

Battery::Battery(Logger* newLogger, bool loglevelSet, QCoreApplication *app, QObject *parent) : QObject(parent)
{
    logger = newLogger;
    settings = new Settings(logger, this);
    const QString notFound = "not found";

    // Read log level from config - if not already set
    if(!loglevelSet) {
        int logLevel = settings->getLogLevel();
        logger->debug = (logLevel == 2);
        logger->verbose = (logLevel > 1);
        logL(QString("Log level set to %1").arg((logLevel == 0 ? "low" : (logLevel == 1 ? "medium" : "high"))));
    }

    chargeNotification = new MyNotification(this);
    healthNotification = new MyNotification(this);

    foreach(const QString& file, this->capacityFiles) {
        if(!chargeFile && QFile::exists(file)) {
            chargeFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery charge file: " + (chargeFile ? chargeFile->fileName() : notFound));

    foreach(const QString& file, this->currentFiles) {
        if(!currentFile && QFile::exists(file)) {
            currentFile = new QFile(file, this);
            break;
        }
    }

    logL("Charging/discharging current file: " + (currentFile ? currentFile->fileName() : notFound));

    foreach(const QString& file, this->maxCurrentFiles) {
        if(!maxChargeCurrentFile && QFile::exists(file)) {
            maxChargeCurrentFile = new QFile(file, this);
            break;
        }
    }

    if(maxChargeCurrentFile) {
        logL("Max charge current file: " + maxChargeCurrentFile->fileName());
        if(maxChargeCurrentFile->open(QIODevice::WriteOnly)) {
            maxChargeCurrentFile->close();
            if(maxChargeCurrentFile->open(QIODevice::ReadOnly)) {

                // Read and store the default max current
                maxSupportedChargeCurrent = maxChargeCurrentFile->readLine().trimmed().toInt();
                logL(QString("Maximum supported charge current: %1mA").arg(maxSupportedChargeCurrent / 1000));
                maxChargeCurrentFile->close();
                settings->setMaxSupportedChargeCurrent(maxSupportedChargeCurrent);

                // Read and maybe set the user-set max current
                maxChargeCurrent = settings->getMaxChargeCurrent();
                if(maxChargeCurrent != maxSupportedChargeCurrent) {
                    setMaxChargeCurrent(maxChargeCurrent);
                }
            }
        }
        else {
            logL("Max charge current file is not writable - feature disabled");
            delete maxChargeCurrentFile;
            maxChargeCurrentFile = Q_NULLPTR;
        }
    }
    else {
        logL("Max charge current file: " + notFound);
    }

    foreach(const QString& file, this->statusFiles) {
        if(!stateFile && QFile::exists(file)) {
            stateFile = new QFile(file, this);
            break;
        }
    }

    logL("Status file: " + (stateFile ? stateFile->fileName() : notFound));

    foreach(const QString& file, this->chargerFiles) {
        if(!chargerConnectedFile && QFile::exists(file)) {
            chargerConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("Charger status file: " + (chargerConnectedFile ? chargerConnectedFile->fileName() : notFound));

    foreach(const QString& file, this->acFiles) {
        if(!acConnectedFile && QFile::exists(file)) {
            acConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("AC status file: " + (acConnectedFile ? acConnectedFile->fileName() : notFound));

    foreach(const QString& file, this->tempFiles) {
        if(!temperatureFile && QFile::exists(file)) {
            temperatureFile = new QFile(file, this);
            break;
        }
    }

    // e.g. PineTab outputs an integer in centi-centigrade
    // Note that the formatter in the QML page, and the logger divide by 10 again!
    if(temperatureFile->fileName().contains(QStringLiteral("xp20x-battery"))) {
        tempCorrectionFactor = 10.0;
    }

    logL("Battery temperature file: " + (temperatureFile ? temperatureFile->fileName() : notFound));

    foreach(const QString& file, this->healthFiles) {
        if(!healthFile && QFile::exists(file)) {
            healthFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery health file: " + (healthFile ? healthFile->fileName() : notFound));

    foreach(const QString& file, this->controlFiles) {
        if(!chargingEnabledFile && QFile::exists(file)) {
            chargingEnabledFile = new QFile(file, this);
            break;
        }
    }

    // Flip the charging control bits if necessary
    if(chargingEnabledFile && chargingEnabledFile->fileName().contains("enable")) {
        enableChargingValue = 1;
        disableChargingValue = 0;
    }

    // If we found a usable file, check that it is writable
    if(chargingEnabledFile) {
        logL("Charger control file: " + chargingEnabledFile->fileName());
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            chargingEnabledFile->close();
        }
        else {
            logL("Charger control file is not writable - feature disabled");
            delete chargingEnabledFile;
            chargingEnabledFile = Q_NULLPTR;
        }
    }
    else {
        logL("Charger control file not found!");
        logL("Please contact the developer with your device model!");
    }

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
    setMaxChargeCurrent(maxSupportedChargeCurrent);

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
    if(chargeFile && chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readLine().trimmed().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            logM(QString("Battery: %1%").arg(charge));
        }
        chargeFile->close();
    }

    if(chargerConnectedFile && chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            logM(QString("Charger: %1").arg(chargerConnected ? "connected" : "disconnected"));
        }
        chargerConnectedFile->close();
    }

    if(acConnectedFile && acConnectedFile->open(QIODevice::ReadOnly)) {
        nextAcConnected = acConnectedFile->readLine().trimmed().toInt();
        if(nextAcConnected != acConnected) {
            acConnected = nextAcConnected;
            logM(QString("AC: %1").arg(acConnected ? "connected" : "disconnected"));
        }
        acConnectedFile->close();
    }

    if(currentFile && currentFile->open(QIODevice::ReadOnly)) {
        current = currentFile->readLine().trimmed().toInt();
        if(!invertDecided) {
            invertCurrent = (!chargerConnected && !acConnected && current > 10);
            if(invertCurrent) logL("Battery current inverted");
            else              logL("Battery current not inverted");
            invertDecided = true;
        }
        current = current * (invertCurrent ? -1 : 1);
        logH(QString("Current: %1mA").arg(current / 1000));
        currentFile->close();
    }

    if(stateFile && stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            logM("State: " + state);

            // Hide/show notification right away
            resetTimers();
        }
        stateFile->close();
    }

    if(temperatureFile && temperatureFile->open(QIODevice::ReadOnly)) {
        nextTemperature = temperatureFile->readLine().trimmed().toInt() / tempCorrectionFactor;
        if(nextTemperature != temperature) {
            if((nextTemperature / 10) != (temperature / 10)) {
                logM(QString("Temperature: %1°C").arg(nextTemperature / 10));
            }
            temperature = nextTemperature;
        }
        temperatureFile->close();
    }

    if(healthFile && healthFile->open(QIODevice::ReadOnly)) {
        nextHealth = (QString(healthFile->readLine().trimmed().toLower()));
        if(nextHealth != health) {
            health = nextHealth;
            logM("Health: " + health);

            // Hide/show notification right away
            resetTimers();
        }
        healthFile->close();
    }

    if(chargingEnabledFile && settings->getLimitEnabled()) {
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

int Battery::getCharge() { return charge; }

QString Battery::getState() { return state; }

int Battery::getTemperature() { return temperature; }

QString Battery::getHealth() { return health; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::setChargingEnabled(const bool isEnabled) {
    bool success = false;
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
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
            chargingEnabledFile->close();
        }
        else {
            logL("Could not open charger control file");
        }
    }
    return success;
}

void Battery::setMaxChargeCurrent(int newCurrent) {
    if(maxChargeCurrentFile) {
        logM(QString("Max charging current: %1mA").arg(newCurrent / 1000));
        if(newCurrent > maxSupportedChargeCurrent) {
            newCurrent = maxSupportedChargeCurrent;
        }
        if(maxChargeCurrentFile->open(QIODevice::WriteOnly)) {
            QString data = QString("%1").arg(newCurrent);
            if(!maxChargeCurrentFile->write(data.toLocal8Bit())) {
                logM("Could not write to max charging current file");
            }
        }
        else {
            logM("Could not open max charging current file");
        }
        maxChargeCurrentFile->close();
    }
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}

bool Battery::getAcConnected() {
    return acConnected;
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
