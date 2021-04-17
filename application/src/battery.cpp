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

Battery::Battery(Settings* newSettings, Logger* newLogger, QObject* parent) : QObject(parent)
{
    QString filename;
    settings = newSettings;
    logger = newLogger;

    // Number: charge percentage, e.g. 42
    chargeFile   = new QFile("/sys/class/power_supply/battery/capacity", this);
    logV("Capacity file: " + chargeFile->fileName());

    // Number: battery/charging current, e.g. -1450000 (-145mA)
    currentFile   = new QFile("/sys/class/power_supply/battery/current_now", this);
    logV("Charge state file: " + currentFile->fileName());

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile   = new QFile("/sys/class/power_supply/battery/status", this);
    logV("Charger status file: " + stateFile->fileName());

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    logV("Reading charger status from" + chargerConnectedFile->fileName());

    // ENABLE/DISABLE CHARGING
    if(QHostInfo::localHostName().contains("SailfishEmul")) {
        logV("Sailfish SDK detected, not using charger control file");
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
            logE("Charger control file not found!");
            logE("Please contact the developer with your device model!");
        }
    }

    // If we found a usable file, check that it is writable
    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            logV("Controlling charging via" + chargingEnabledFile->fileName());
            chargingEnabledFile->close();
        }
        else {
            logE("Charger control file" + chargingEnabledFile->fileName() + "is not writable");
            logE("Charger control feature disabled");
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
            logV(QString("Battery: %1").arg(charge));
        }
        chargeFile->close();
    }
    if(currentFile && currentFile->open(QIODevice::ReadOnly)) {
        nextCurrent = currentFile->readLine().trimmed().toInt();
        if(nextCurrent != current) {
            current = nextCurrent;
            emit currentChanged(current);
            logV(QString("Current: %1").arg(current));
        }
        currentFile->close();
    }
    if(chargerConnectedFile && chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            logV(QString("Charger is connected: %1").arg(chargerConnected));
        }
        chargerConnectedFile->close();
    }
    if(stateFile && stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            logV(QString("Charging status: %1").arg(state));
        }
        stateFile->close();
    }
}

int Battery::getCharge(){ return charge; }

int Battery::getCurrent(){ return current; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

void Battery::setChargingEnabled(const bool isEnabled) {
    if(chargingEnabledFile && chargingEnabledFile->open(QIODevice::WriteOnly)) {
        if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
            chargingEnabled = isEnabled;
            emit chargingEnabledChanged(chargingEnabled);

            if(isEnabled) {
                logD("Charging resumed");
            }
            else {
                logD("Charging paused");
            }
        }
        chargingEnabledFile->close();
    }
}

bool Battery::getChargerConnected() {
    return chargerConnected;
}
