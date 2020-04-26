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

Battery::Battery(Settings* newSettings, QObject* parent) : QObject(parent)
{
    QString filename;
    settings = newSettings;

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
            qInfo() << "Controlling charging via" << chargingEnabledFile->fileName();
            chargingEnabledFile->close();
        }
        else {
            qWarning() << "Charger control file" << chargingEnabledFile->fileName() << "is not writable";
            qWarning() << "Charger control feature disabled";
            delete chargingEnabledFile;
            chargingEnabledFile = Q_NULLPTR;
        }
    }

    // TODO
    // Use DBus instead of polling the files - where is the documentation?
    // QFileWatcher doesn't work, because e.g. the charge file
    // isn't a real file, and it doesn't "change" when the value changes.

    updateData();
}

Battery::~Battery() { }

void Battery::updateData()
{
    if(chargeFile && chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readLine().trimmed().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            emit chargeChanged(charge);
            qDebug() << "Battery:" << charge;
        }
        chargeFile->close();
    }
    if(chargerConnectedFile && chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            qDebug() << "Charger is connected:" << chargerConnected;
        }
        chargerConnectedFile->close();
    }
    if(stateFile && stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            qDebug() << "Charging status:" << state;
        }
        stateFile->close();
    }
}

int Battery::getCharge(){ return charge; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

void Battery::setChargingEnabled(bool isEnabled) {
    if(chargingEnabledFile && chargingEnabledFile->open(QIODevice::WriteOnly)) {
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
        chargingEnabledFile->close();
    }
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}
