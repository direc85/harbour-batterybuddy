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
#include "devices.h"
#include "battery.h"
#include <QDateTime>

Battery::Battery(Settings* newSettings, Logger* newLogger, QObject* parent) : QObject(parent)
{
    settings = newSettings;
    logger = newLogger;
    const QString notFound = "not found";

    foreach(const QString& file, capacityFiles) {
        if(!chargeFile && QFile::exists(file)) {
            chargeFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery charge file: " + (chargeFile ? chargeFile->fileName() : notFound));

    foreach(const QString& file, currentFiles) {
        if(!currentFile && QFile::exists(file)) {
            currentFile = new QFile(file, this);
            break;
        }
    }

    logL("Charging/discharging current file: " + (currentFile ? currentFile->fileName() : notFound));

    foreach(const QString& file, statusFiles) {
        if(!stateFile && QFile::exists(file)) {
            stateFile = new QFile(file, this);
            break;
        }
    }

    logL("Status file: " + (stateFile ? stateFile->fileName() : notFound));

    foreach(const QString& file, usbPresentFiles) {
        if(!chargerConnectedFile && QFile::exists(file)) {
            chargerConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("Charger status file: " + (chargerConnectedFile ? chargerConnectedFile->fileName() : notFound));

    foreach(const QString& file, acPresentFiles) {
        if(!acConnectedFile && QFile::exists(file)) {
            acConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("AC status file: " + (acConnectedFile ? acConnectedFile->fileName() : notFound));

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

    foreach(const QString& file, timeToFullFiles) {
        if(!timeToFullFile && QFile::exists(file)) {
            timeToFullFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery time-to-full file: " + (timeToFullFile ? timeToFullFile->fileName() : notFound));

    foreach(const QString& file, healthFiles) {
        if(!healthFile && QFile::exists(file)) {
            healthFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery health file: " + (healthFile ? healthFile->fileName() : notFound));

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

    if(timeToFullFile && timeToFullFile->open(QIODevice::ReadOnly)) {
        int nextTimeToFull = timeToFullFile->readLine().trimmed().toInt();
        if(nextTimeToFull != timeToFull) {
            // C2 reports 1 when not connected, report it as 'unknown'
            if(nextTimeToFull == 1) nextTimeToFull = 0x7FFFFFFF;
            timeToFull = nextTimeToFull;
            emit timeToFullChanged(timeToFull);
            logH(QString("Time to full: %1").arg(QDateTime::fromTime_t(timeToFull).toString("HH mm")));
        }
        timeToFullFile->close();
    }

}

int Battery::getCharge() { return charge; }

int Battery::getCurrent() { return current; }

int Battery::getMaxChargeCurrent() { return maxChargeCurrent; }

QString Battery::getState() { return state; }

QString Battery::getHealth() { return health; }

int Battery::getTemperature(){ return temperature; }

int Battery::getTimeToFull(){ return timeToFull; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

bool Battery::getChargerConnected() { return chargerConnected; }

bool Battery::getAcConnected() { return acConnected; }
