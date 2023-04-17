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

Battery::Battery(Settings* newSettings, Logger* newLogger, QObject* parent) : QObject(parent)
{
    settings = newSettings;
    logger = newLogger;
    const QString notFound = "not found";

    // Battery charge percentage, number, e.g. 42
    const QStringList batteryFiles = {
        "/sys/class/power_supply/battery/capacity",
        "/sys/class/power_supply/dollar_cove_battery/capacity",
        "/sys/class/power_supply/axp20x-battery/capacity"
    };

    foreach(const QString& file, batteryFiles) {
        if(!chargeFile && QFile::exists(file)) {
            chargeFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery charge file: " + (chargeFile ? chargeFile->fileName() : notFound));

    // Number: battery/charging current, e.g. -1450000 (-145mA)
    const QStringList currentFiles = {
        "/sys/class/power_supply/battery/current_now",
        "/sys/class/power_supply/dollar_cove_battery/current_now",
        "/sys/class/power_supply/axp20x-battery/current_now"
    };

    foreach(const QString& file, currentFiles) {
        if(!currentFile && QFile::exists(file)) {
            currentFile = new QFile(file, this);
            break;
        }
    }

    logL("Charging/discharging current file: " + (currentFile ? currentFile->fileName() : notFound));

    // String: charging, discharging, full, empty, unknown (others?)
    const QStringList stateFiles = {
        "/sys/class/power_supply/battery/status",
        "/sys/class/power_supply/dollar_cove_battery/status",
        "/sys/class/power_supply/axp20x-battery/status"
    };

    foreach(const QString& file, stateFiles) {
        if(!stateFile && QFile::exists(file)) {
            stateFile = new QFile(file, this);
            break;
        }
    }

    logL("Status file: " + (stateFile ? stateFile->fileName() : notFound));

    // Number: 0 or 1
    const QStringList usbPresentFiles = {
        "/sys/class/power_supply/usb/present",
        "/sys/class/power_supply/dollar_cove_charger/present",
        "/sys/class/power_supply/axp20x-usb/present"
    };

    foreach(const QString& file, usbPresentFiles) {
        if(!chargerConnectedFile && QFile::exists(file)) {
            chargerConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("Charger status file: " + (chargerConnectedFile ? chargerConnectedFile->fileName() : notFound));

    // Number: 0 or 1
    const QStringList acPresentFiles = {
        "/sys/class/power_supply/ac/present",
        "/sys/class/power_supply/axp813-ac/present"
    };

    foreach(const QString& file, acPresentFiles) {
        if(!acConnectedFile && QFile::exists(file)) {
            acConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("AC status file: " + (acConnectedFile ? acConnectedFile->fileName() : notFound));

    // Number: temperature
    const QStringList tempFiles = {
        "/sys/class/power_supply/battery/temp",
        "/sys/class/power_supply/dollar_cove_battery/temp",
        "/sys/class/power_supply/axp20x-battery/hwmon0/in0_input"
    };

    foreach(const QString& file, tempFiles) {
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

    // String: health state
    const QStringList healthFiles = {
        "/sys/class/power_supply/battery/health",
        "/sys/class/power_supply/dollar_cove_battery/health",
        "/sys/class/power_supply/axp20x-battery/health"
    };

    foreach(const QString& file, healthFiles) {
        if(!healthFile && QFile::exists(file)) {
            healthFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery health file: " + (healthFile ? healthFile->fileName() : notFound));

    // Charger control file
    const QStringList controlFiles = {
        "/sys/class/power_supply/battery/input_suspend",              // e.g. Sony Xperia XA2
        "/sys/class/power_supply/battery/charging_enabled",           // e.g. for Sony Xperia Z3 Compact Tablet
        "/sys/class/power_supply/usb/charger_disable",                // e.g. for Jolla Phone
        "/sys/class/power_supply/dollar_cove_battery/enable_charging" // e.g. for Jolla Tablet
    };

    foreach(const QString& file, controlFiles) {
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
        logL("Charger control file: " + notFound);
        logL("Please contact the developer with your device model!");
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
            logM(QString("Battery: %1%").arg(charge));
        }
        chargeFile->close();
    }

    if(chargerConnectedFile && chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged(chargerConnected);
            logM(QString("Charger: %1").arg(chargerConnected ? "connected" : "disconnected"));
        }
        chargerConnectedFile->close();
    }

    if(acConnectedFile && acConnectedFile->open(QIODevice::ReadOnly)) {
        nextAcConnected = acConnectedFile->readLine().trimmed().toInt();
        if(nextAcConnected != acConnected) {
            acConnected = nextAcConnected;
            emit acConnectedChanged(acConnected);
            logM(QString("AC: %1").arg(acConnected ? "connected" : "disconnected"));
        }
        acConnectedFile->close();
    }

    if(stateFile && stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged(state);
            logM("State: " + state);
        }
        stateFile->close();
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
        emit currentChanged(current);
        logH(QString("Current: %1mA").arg(current / 1000));
        currentFile->close();
    }

    if(healthFile && healthFile->open(QIODevice::ReadOnly)) {
        nextHealth = (QString(healthFile->readLine().trimmed().toLower()));
        if(nextHealth != health) {
            health = nextHealth;
            emit healthChanged(health);
            logM("Health: " + health);
        }
        healthFile->close();
    }

    if(temperatureFile && temperatureFile->open(QIODevice::ReadOnly)) {
        nextTemperature = temperatureFile->readLine().trimmed().toInt() / tempCorrectionFactor;
        if(nextTemperature != temperature) {
            temperature = nextTemperature;
            emit temperatureChanged(temperature);
            logH(QString("Temperature: %1°C").arg(temperature / 10));
        }
        temperatureFile->close();
    }
}

int Battery::getCharge() { return charge; }

int Battery::getCurrent() { return current; }

int Battery::getMaxChargeCurrent() { return maxChargeCurrent; }

QString Battery::getState() { return state; }

QString Battery::getHealth() { return health; }

int Battery::getTemperature(){ return temperature; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::getChargerConnected() { return chargerConnected; }

bool Battery::getAcConnected() { return acConnected; }
