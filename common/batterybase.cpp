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
#include "batterybase.h"

BatteryBase::BatteryBase(Logger* newLogger, QObject* parent) : QObject(parent)
{
    logger = newLogger;

    const QString notFound = QStringLiteral("not found");

    foreach(const QString& file, acConnectedFiles) {
        if(QFile::exists(file)) {
            acConnectedFile = new QFile(file, this);
            break;
        }
    }
    logL("AC status file: " + (acConnectedFile ? acConnectedFile->fileName() : notFound));

    foreach(const QString& file, chargeFiles) {
        if(QFile::exists(file)) {
            chargeFile = new QFile(file, this);
            break;
        }
    }
    logL("Battery charge file: " + (chargeFile ? chargeFile->fileName() : notFound));

    foreach(const QString& file, controlFiles) {
        if(!controlFile && QFile::exists(file)) {
            controlFile = new QFile(file, this);
            break;
        }
    }
    logL("Charger control file: " + (controlFile ? controlFile->fileName() : notFound));

    foreach(const QString& file, currentFiles) {
        if(QFile::exists(file)) {
            currentFile = new QFile(file, this);
            break;
        }
    }
    logL("Charging/discharging current file: " + (currentFile ? currentFile->fileName() : notFound));

    foreach(const QString& file, healthFiles) {
        if(QFile::exists(file)) {
            healthFile = new QFile(file, this);
            break;
        }
    }
    logL("Battery health file: " + (healthFile ? healthFile->fileName() : notFound));

    foreach(const QString& file, maxCurrentFiles) {
        if(QFile::exists(file)) {
            maxCurrentFile = new QFile(file, this);
            break;
        }
    }
    logL("Max charge current file: " + (maxCurrentFile ? maxCurrentFile->fileName() : notFound));

    foreach(const QString& file, statusFiles) {
        if(QFile::exists(file)) {
            statusFile = new QFile(file, this);
            break;
        }
    }
    logL("Status file: " + (statusFile ? statusFile->fileName() : notFound));

    foreach(const QString& file, usbConnectedFiles) {
        if(QFile::exists(file)) {
            usbConnectedFile = new QFile(file, this);
            break;
        }
    }

    logL("Charger status file: " + (usbConnectedFile ? usbConnectedFile->fileName() : notFound));

    foreach(const QString& file, temperatureFiles) {
        if(QFile::exists(file)) {
            temperatureFile = new QFile(file, this);
            // e.g. PineTab outputs an integer in centi-centigrade
            // Note that the formatter in the QML page, and the logger divide by 10 again!
            if(temperatureFile->fileName().contains(QStringLiteral("xp20x-battery"))) {
                tempCorrectionFactor = 10.0;
            }
            break;
        }
    }

    logL("Battery temperature file: " + (temperatureFile ? temperatureFile->fileName() : notFound));

    foreach(const QString& file, timeToFullFiles) {
        if(QFile::exists(file)) {
            timeToFullFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery timeToFull file: " + (timeToFullFile ? timeToFullFile->fileName() : notFound));

    foreach(const QString& file, healthFiles) {
        if(!healthFile && QFile::exists(file)) {
            healthFile = new QFile(file, this);
            break;
        }
    }

    logL("Battery health file: " + (healthFile ? healthFile->fileName() : notFound));
}

BatteryBase::~BatteryBase() { }

void BatteryBase::updateBaseData()
{
    if(chargeFile && chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readLine().trimmed().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            emit _chargeChanged(charge);
            logM(QString("Battery: %1%").arg(charge));
        }
        chargeFile->close();
    }

    if(usbConnectedFile && usbConnectedFile->open(QIODevice::ReadOnly)) {
        nextUsbConnected = usbConnectedFile->readLine().trimmed().toInt();
        if(nextUsbConnected != usbConnected) {
            usbConnected = nextUsbConnected;
            emit _chargerConnectedChanged(usbConnected);
            logM(QString("Charger: %1").arg(usbConnected ? "connected" : "disconnected"));
        }
        usbConnectedFile->close();
    }

    if(acConnectedFile && acConnectedFile->open(QIODevice::ReadOnly)) {
        nextAcConnected = acConnectedFile->readLine().trimmed().toInt();
        if(nextAcConnected != acConnected) {
            acConnected = nextAcConnected;
            emit _acConnectedChanged(acConnected);
            logM(QString("AC: %1").arg(acConnected ? "connected" : "disconnected"));
        }
        acConnectedFile->close();
    }

    if(statusFile && statusFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(statusFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit _stateChanged(state);
            logM("State: " + state);
        }
        statusFile->close();
    }

    if(currentFile && currentFile->open(QIODevice::ReadOnly)) {
        current = currentFile->readLine().trimmed().toInt();
        if(!invertDecided) {
            bool connected = usbConnected || acConnected;
            if(connected && current <= -200) {
                logL("Battery current inverted");
                invertSign = -1;
                invertDecided = true;
            }
            else if(connected && current >= 200) {
                logL("Battery current not inverted");
                invertSign = 1;
                invertDecided = true;
            }
        }
        current = invertSign * current;
        emit _currentChanged(current);
        logH(QString("Current: %1mA").arg(current / 1000));
        currentFile->close();
    }

    if(healthFile && healthFile->open(QIODevice::ReadOnly)) {
        nextHealth = (QString(healthFile->readLine().trimmed().toLower()));
        if(nextHealth != health) {
            health = nextHealth;
            emit _healthChanged(health);
            logM("Health: " + health);
        }
        healthFile->close();
    }

    if(temperatureFile && temperatureFile->open(QIODevice::ReadOnly)) {
        nextTemperature = temperatureFile->readLine().trimmed().toInt() / tempCorrectionFactor;
        if(nextTemperature != temperature) {
            temperature = nextTemperature;
            emit _temperatureChanged(temperature);
            logH(QString("Temperature: %1°C").arg(temperature / 10));
        }
        temperatureFile->close();
    }

    if(timeToFullFile && timeToFullFile->open(QIODevice::ReadOnly)) {
        int nextTimeToFull = timeToFullFile->readLine().trimmed().toInt();
        if(nextTimeToFull != timeToFull) {
            // C2 reports 1 when not charging:
            timeToFull = (nextTimeToFull == 1) ? 0x7FFFFFFF : nextTimeToFull;
            emit _timeToFullChanged(timeToFull);
            logH(QString("Time to Full: %1s").arg(timeToFull));
        }
        timeToFullFile->close();
    }
}


int BatteryBase::getCharge() { return charge; }
int BatteryBase::getCurrent() { return current; }
int BatteryBase::getMaxChargeCurrent() { return maxChargeCurrent; }
QString BatteryBase::getState() { return state; }
QString BatteryBase::getHealth() { return health; }
int BatteryBase::getTemperature(){ return temperature; }
bool BatteryBase::getChargingEnabled() { return chargingEnabled; }
bool BatteryBase::getUsbConnected() { return usbConnected; }
bool BatteryBase::getAcConnected() { return acConnected; }
int BatteryBase::getTimeToFull(){ return timeToFull; }
