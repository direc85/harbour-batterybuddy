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

Battery::Battery(QObject *parent) : QObject(parent)
{
    QString filename;
    settings = new Settings(this);
    updateTimer = new QTimer(this);
    highNotifyTimer = new QTimer(this);
    lowNotifyTimer = new QTimer(this);
    notification = new MyNotification(this);

    // Number: charge percentage, e.g. 42
    chargeFile   = new QFile("/sys/class/power_supply/battery/capacity", this);
    qInfo() << "Capacity file: " << chargeFile->fileName();

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile   = new QFile("/sys/class/power_supply/battery/status", this);
    qInfo() << "Charge state file: " << stateFile->fileName();

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    qInfo() << "Charger status file: " << chargerConnectedFile->fileName();

    // ENABLE/DISABLE CHARGING
    if(QHostInfo::localHostName().contains("SailfishEmul")) {
        qInfo() << "Sailfish SDK detected, not using charger control file";
    }
    else {
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

        if(!chargingEnabledFile) {
            qWarning() << "Charger control file not found!";
            qWarning() << "Please contact the developer with your device model!";
        }
    }

    // If we found a usable file, check that it is writable
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            chargingEnabledFile->close();
        }
        else {
            qWarning() << "Charger control file" << chargingEnabledFile->fileName() << "is not writable";
        }
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
            qDebug() << "Battery:" << charge;
        }
        chargeFile->close();
    }
    if(chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            qDebug() << "Charger is connected:" << chargerConnected;
        }
        chargerConnectedFile->close();
    }
    if(stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            qDebug() << "Charging status:" << state;

            // Hide/show notification right away
            resetTimers();
        }
        stateFile->close();
    }
    if(chargingEnabledFile && settings->getLimitEnabled()) {
        if(chargingEnabled && charge >= settings->getHighLimit()) {
            qDebug() << "Disabling";
            setChargingEnabled(false);
        }
        else if(!chargingEnabled && charge <= settings->getLowLimit()) {
            qDebug() << "Enabling";
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
        qDebug() << "Starting high level notification timer";
        highNotifyTimer->start();
        showHighNotification();
    }
    else {
        qDebug() << "High level notification timer not started";
    }
    if(settings->getLowNotificationsInterval() < 610) {
        qDebug() << "Starting low level notification timer";
        lowNotifyTimer->start();
        showLowNotification();
    }
    else {
        qDebug() << "Low level notification timer not started";
    }
}

void Battery::showHighNotification() {
    if(settings->getHighNotificationsInterval() < 610 && (charge >= settings->getHighAlert() && state != "discharging")
            && !(charge == 100 && state == "idle")) {
        qDebug() << "High notification timer: full enough battery";
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationHighText(), settings->getHighAlertFile());
        if(settings->getHighNotificationsInterval() == 50) {
            qDebug() << "Stop high notification timer (show only once)";
            highNotifyTimer->stop();
        }
    }
    else if(charge > settings->getLowAlert()) {
        qDebug() << "High notification timer: close notification";
        notification->close();
    }
}

void Battery::showLowNotification() {
    if(settings->getLowNotificationsInterval() < 610 && charge <= settings->getLowAlert() && state != "charging") {
        qDebug() << "Battery notification timer: empty enough battery";
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationLowText(), settings->getLowAlertFile());
        if(settings->getLowNotificationsInterval() == 50) {
            qDebug() << "Stop low notification timer (show only once)";
            lowNotifyTimer->stop();
        }
    }
    else if(charge < settings->getHighAlert()) {
        qDebug() << "Low notification timer: close notification";
        notification->close();
    }
}

int Battery::getCharge() { return charge; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::setChargingEnabled(bool isEnabled) {
    bool success = false;
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
                chargingEnabled = isEnabled;
                emit chargingEnabledChanged(chargingEnabled);
                success = true;

                if(isEnabled) {
                    qInfo() << "Charging resumed";
                }
                else {
                    qInfo() << "Charging paused";
                }
            }
            else {
                qWarning() << "Could not write new charger state";
            }
            chargingEnabledFile->close();
        }
        else {
            qWarning() << "Could not open charger control file";
        }
    }
    return success;
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}

void Battery::shutdown() {
    qDebug() << "Preparing for exit...";
    blockSignals(true);
    if(updateTimer) {
        updateTimer->stop();
        qDebug() << "Update timer stopped";
    }
    notification->close();
    if(highNotifyTimer) {
        highNotifyTimer->stop();
        qDebug() << "High battery notification stopped";
    }
    if(lowNotifyTimer) {
        lowNotifyTimer->stop();
        qDebug() << "Low battery notification stopped";
    }
    // ENABLE/DISABLE CHARGING
    if(!QHostInfo::localHostName().contains("SailfishEmul") && !setChargingEnabled(true)) {
        qWarning() << "ERROR! Could not restore charger status! Your device" << endl
                   << "may not start until reboot! If that doesn't help," << endl
                   << "uninstall Battery Buddy and reboot your device.";
    }
}
