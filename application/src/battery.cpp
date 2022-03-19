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
    settings = newSettings;
    logger = newLogger;

    // Number: charge percentage, e.g. 42
    chargeFile   = new QFile("/sys/class/power_supply/battery/capacity", this);
    logE("Capacity file: " + chargeFile->fileName());

    // Number: battery/charging current, e.g. -1450000 (-145mA)
    currentFile   = new QFile("/sys/class/power_supply/battery/current_now", this);
    logE("Charge state file: " + currentFile->fileName());

    // String: charging, discharging, full, empty, unknown (others?)
    stateFile   = new QFile("/sys/class/power_supply/battery/status", this);
    logE("Charger status file: " + stateFile->fileName());

    // Number: 0 or 1
    chargerConnectedFile = new QFile("/sys/class/power_supply/usb/present", this);
    logE("Reading charger status from" + chargerConnectedFile->fileName());

    QString filename;

    // Number: temperature
    filename = "/sys/class/power_supply/battery/temp";
    if(!temperatureFile && QFile::exists(filename)) {
        temperatureFile = new QFile(filename, this);
    }

    // String: health state
    filename = "/sys/class/power_supply/battery/health";
    if(!healthFile && QFile::exists(filename)) {
        healthFile = new QFile(filename, this);
    }

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

    if(!chargingEnabledFile && !QSysInfo::machineHostName().contains("SailfishEmul")) {
        logE("Charger control file not found!");
        logE("Please contact the developer with your device model!");
    }

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
            logV(QString("Battery: %1%").arg(charge));
        }
        chargeFile->close();
    }
    if(chargerConnectedFile && chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            logV(QString("Charger: %1").arg(chargerConnected ? "connected" : "disconnected"));
        }
        chargerConnectedFile->close();
    }
    if(stateFile && stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            logV("State: " + state);
        }
        stateFile->close();
    }
    if(currentFile && currentFile->open(QIODevice::ReadOnly)) {
        nextCurrent = currentFile->readLine().trimmed().toInt();
        if(nextCurrent != current) {
            current = nextCurrent;
            emit currentChanged(current);
            logD(QString("Current: %1mA").arg(current / 1000));
        }
        currentFile->close();
    }
    if(healthFile && healthFile->open(QIODevice::ReadOnly)) {
        nextHealth = (QString(healthFile->readLine().trimmed().toLower()));
        if(nextHealth != health) {
            health = nextHealth;
            emit healthChanged(health);
            logV("Health: " + health);
        }
        healthFile->close();
    }
    if(temperatureFile && temperatureFile->open(QIODevice::ReadOnly)) {
        nextTemperature = temperatureFile->readLine().trimmed().toInt();
        if(nextTemperature != temperature) {
            temperature = nextTemperature;
            emit temperatureChanged(temperature);
            // TODO: factor might be different depending on device
            // X10 has degrees * 10
            logD(QString("Temperature: %1°C").arg(temperature / 10));
        }
        temperatureFile->close();
    }
}

int Battery::getCharge(){ return charge; }

int Battery::getCurrent(){ return current; }

QString Battery::getState() { return state; }

QString Battery::getHealth() { return health; }

int Battery::getTemperature(){ return temperature; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::getChargerConnected() { return chargerConnected; }
