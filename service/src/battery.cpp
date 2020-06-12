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

Battery::Battery(Settings *newSettings, QTimer *newUpdater, QTimer *newNotifier, Notification *newNotification, QObject *parent) : QObject(parent)
{
    QString filename;
    settings = newSettings;
    updateTimer = newUpdater;
    notifyTimer = newNotifier;
    notification = newNotification;

    // Number: charge percentage, e.g. 42
    chargeFile   = new QFile("/sys/class/power_supply/battery/capacity", this);
    qInfo() << "Reading capacity from" << chargeFile->fileName();

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile   = new QFile("/sys/class/power_supply/battery/status", this);
    qInfo() << "Reading charge state from" << stateFile->fileName();

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    qInfo() << "Reading charger status from" << chargerConnectedFile->fileName();

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

    updateData();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    connect(settings, SIGNAL(configChanged()), this, SLOT(updateConfig()));
    connect(notifyTimer, SIGNAL(timeout()), this, SLOT(showNotification()));

    updateConfig();

    updateTimer->setInterval(5000);
    updateTimer->start();

    if(settings->getNotificationsEnabled()) {
        showNotification();
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

void Battery::updateConfig() {
    notifyTimer->stop();
    notifyTimer->setInterval(settings->getInterval() * 1000);
    if(settings->getNotificationsEnabled())
        notifyTimer->start();
}

void Battery::showNotification() {
    if(!settings->getNotificationsEnabled())
        return;

    qInfo() << "battery" << charge << "low" << settings->getLowAlert() << "high" << settings->getHighAlert() << "state" << state;

    if(charge <= settings->getLowAlert() && state != "charging") {
        qDebug() << "Battery notification timer: empty enough battery";
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationLowText(), settings->getLowAlertFile());
    }
    else if((charge >= settings->getHighAlert() && state != "discharging")
            && !(charge == 100 && state == "idle")) {
        qDebug() << "Battery notification timer: full enough battery";
        notification->send(settings->getNotificationTitle().arg(charge), settings->getNotificationHighText(), settings->getHighAlertFile());
    }
    else {
        qDebug() << "Battery notification timer: close notification";
        notification->close();
    }
}
int Battery::getCharge() { return charge; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

void Battery::setChargingEnabled(bool isEnabled) {
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
                chargingEnabled = isEnabled;
                emit chargingEnabledChanged(chargingEnabled);

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
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}

void Battery::shutdown() {
    qDebug() << "\nPreparing for exit...";
    blockSignals(true);
    if(updateTimer) {
        updateTimer->stop();
        qDebug() << "Timer stopped";
    }
    notification->close();
    if(notifyTimer) {
        notifyTimer->stop();
        qDebug() << "Notification stopped";
    }
    setChargingEnabled(true);
    chargingEnabledFile->setPermissions(originalPerms);
    qDebug() << "Charger control file permissions restored.";
}
