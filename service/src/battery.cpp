/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2020 Matti Viljanen
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

Battery::Battery(Logger* newLogger, bool loglevelSet, QObject *parent) : QObject(parent)
{
    logger = newLogger;
    settings = new Settings(logger, this);

    // Read log level from config - if not already set
    if(!loglevelSet) {
        int logLevel = settings->getLogLevel();
        logger->debug = (logLevel == 2);
        logger->verbose = (logLevel > 1);
        logE(QString("Log level set to %1").arg((logLevel == 0 ? "low" : (logLevel == 1 ? "medium" : "high"))));
    }

    updateTimer = new QTimer(this);
    highNotifyTimer = new QTimer(this);
    lowNotifyTimer = new QTimer(this);
    notification = new MyNotification(this);

    // Number: charge percentage, e.g. 42
    chargeFile   = new QFile("/sys/class/power_supply/battery/capacity", this);
    logE("Capacity file: " + chargeFile->fileName());

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile   = new QFile("/sys/class/power_supply/battery/status", this);
    logE("Charge state file: " + stateFile->fileName());

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    logE("Charger status file: " + chargerConnectedFile->fileName());

    // ENABLE/DISABLE CHARGING
    QString filename;

    // e.g. for Sony Xperia XA2
    filename = "/sys/class/power_supply/battery/input_suspend";
    if(!chargingEnabledFile && QFile::exists(filename)) {
        chargingEnabledFile = new QFile(filename, this);
        enableChargingValue = 0;
        disableChargingValue = 1;
    }

    // e.g. for Sony Xperia Z3 Compact Tablet
    filename = "/sys/class/power_supply/battery/charging_enabled";
    if(!chargingEnabledFile && QFile::exists(filename)) {
        chargingEnabledFile = new QFile(filename, this);
        enableChargingValue = 1;
        disableChargingValue = 0;
    }

    // e.g. for Jolla Phone
    filename = "/sys/class/power_supply/usb/charger_disable";
    if(!chargingEnabledFile && QFile::exists(filename)) {
        chargingEnabledFile = new QFile(filename, this);
        enableChargingValue = 0;
        disableChargingValue = 1;
    }

    // If we found a usable file, check that it is writable
    if(chargingEnabledFile) {
        logE("Charger control file: " + chargingEnabledFile->fileName());
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            chargingEnabledFile->close();
        }
        else {
            logE("Charger control file" + chargingEnabledFile->fileName() + "is not writable");
            logE("Charger control feature disabled");
            delete chargingEnabledFile;
            chargingEnabledFile = Q_NULLPTR;
        }
    }
    else if(!QHostInfo::localHostName().contains("SailfishEmul")) {
        logE("Charger control file not found!");
        logE("Please contact the developer with your device model!");
    }


    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    connect(settings, SIGNAL(resetTimers()), this, SLOT(resetTimers()));
    connect(highNotifyTimer, SIGNAL(timeout()), this, SLOT(showHighNotification()));
    connect(lowNotifyTimer, SIGNAL(timeout()), this, SLOT(showLowNotification()));

    updateData();
    updateTimer->start(5000);

    // If updateData() didn't start the timers
    // aka. "charging" status didn't change
    // (or if both times are disabled, actually)
    //  manually trigger the timer startup.
    if(!highNotifyTimer->isActive() && !lowNotifyTimer->isActive()) {
        resetTimers();
    }
}

Battery::~Battery() { }

void Battery::updateData()
{
    if(chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readLine().trimmed().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            emit chargeChanged(charge);
            logV(QString("Battery: %1%").arg(charge));
        }
        chargeFile->close();
    }
    if(chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            logV(QString("Charger: %1").arg(chargerConnected ? "connected" : "disconnected"));
        }
        chargerConnectedFile->close();
    }
    if(stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            logV("State: " + state);

            // Hide/show notification right away
            resetTimers();
        }
        stateFile->close();
    }

    if(chargingEnabledFile && settings->getLimitEnabled()) {
        if(chargingEnabled && charge >= settings->getHighLimit()) {
            logD("Disabling charging...");
            setChargingEnabled(false);
        }
        else if(!chargingEnabled && charge <= settings->getLowLimit()) {
            logD("Enabling charging...");
            setChargingEnabled(true);
        }
    }
}

void Battery::resetTimers() {
    highNotifyTimer->stop();
    lowNotifyTimer->stop();
    highNotifyTimer->setInterval(settings->getHighNotificationsInterval() * 1000);
    lowNotifyTimer->setInterval(settings->getLowNotificationsInterval() * 1000);

    if(settings->getHighNotificationsInterval() < 610) {
        logD("Starting high battery timer");
        highNotifyTimer->start();
        showHighNotification();
    }
    else {
        logD("High battery timer not started");
    }

    if(settings->getLowNotificationsInterval() < 610) {
        logD("Start low battery timer");
        lowNotifyTimer->start();
        showLowNotification();
    }
    else {
        logD("Low battery timer not started");
    }
}

void Battery::showHighNotification() {
    if(settings->getHighNotificationsInterval() < 610 && (charge >= settings->getHighAlert() && state != "discharging")
            && !(charge == 100 && state == "idle")) {
        logV(QString("Notification: %1").arg(settings->getNotificationTitle().arg(charge)));
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationHighText(), settings->getHighAlertFile());
        if(settings->getHighNotificationsInterval() == 50) {
            logD("Stop high battery timer");
            highNotifyTimer->stop();
        }
    }
    else if(charge > settings->getLowAlert()) {
        logD("Close high battery notification");
        notification->close();
    }
}

void Battery::showLowNotification() {
    if(settings->getLowNotificationsInterval() < 610 && charge <= settings->getLowAlert() && state != "charging") {
        logV(QString("Notification: %1").arg(settings->getNotificationTitle().arg(charge)));
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationLowText(), settings->getLowAlertFile());
        if(settings->getLowNotificationsInterval() == 50) {
            logD("Stop low battery timer");
            lowNotifyTimer->stop();
        }
    }
    else if(charge < settings->getHighAlert()) {
        logD("Close low battery notification");
        notification->close();
    }
}

int Battery::getCharge() { return charge; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::setChargingEnabled(const bool isEnabled) {
    bool success = false;
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
                chargingEnabled = isEnabled;
                emit chargingEnabledChanged(chargingEnabled);
                success = true;

                if(isEnabled) {
                    logV("Charging resumed");
                }
                else {
                    logV("Charging paused");
                }
            }
            else {
                logE("Could not write new charger state");
            }
            chargingEnabledFile->close();
        }
        else {
            logE("Could not open charger control file");
        }
    }
    return success;
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}

void Battery::shutdown() {
    logV("Shutting down...");
    notification->close();
    blockSignals(true);
    if(updateTimer) {
        updateTimer->stop();
        logD("Update timer stopped");
    }
    if(highNotifyTimer) {
        highNotifyTimer->stop();
        logD("High battery notification stopped");
    }
    if(lowNotifyTimer) {
        lowNotifyTimer->stop();
        logD("Low battery notification stopped");
    }
    // ENABLE/DISABLE CHARGING
    if(!setChargingEnabled(true) && !QHostInfo::localHostName().contains("SailfishEmul")) {
        logE("ERROR! Could not restore charger status! Your device "
             "may not charge until reboot! If that doesn't help, "
             "uninstall Battery Buddy and reboot your device.");
    }
}
