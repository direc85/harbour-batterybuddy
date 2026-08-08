// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021-2025 Peter G. <sailfish@nephros.org>

#include "batterybase.h"

BatteryBase::BatteryBase(Logger* newLogger, QObject* parent)
    : QObject(parent)
    , logger(newLogger)
{
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

    // No point checking the restriction control file
    // if the max current file wasn't writable
    if (maxCurrentFile) {
        foreach(const QString& file, restrictCurrentFiles) {
            if(QFile::exists(file)) {
                restrictCurrentFile = new QFile(file, this);
                break;
            }
        }
        logL("Charge restriction file: " + (restrictCurrentFile ? restrictCurrentFile->fileName() : notFound));
    }

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

    logL("Battery time-to-full file: " + (timeToFullFile ? timeToFullFile->fileName() : notFound));

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
        int nextCharge = chargeFile->readLine().trimmed().toInt();
        chargeFile->close();

        if(nextCharge != charge) {
            charge = nextCharge;
            emit _chargeChanged(charge);
            logM(QString("Battery: %1%").arg(charge));
        }
    }

    if(usbConnectedFile && usbConnectedFile->open(QIODevice::ReadOnly)) {
        bool nextUsbConnected = usbConnectedFile->readLine().trimmed().toInt();
        usbConnectedFile->close();

        if(nextUsbConnected != usbConnected) {
            usbConnected = nextUsbConnected;
            emit _chargerConnectedChanged(usbConnected);
            logM(QString("Charger: %1").arg(usbConnected ? "connected" : "disconnected"));
        }
    }

    if(acConnectedFile && acConnectedFile->open(QIODevice::ReadOnly)) {
        bool nextAcConnected = acConnectedFile->readLine().trimmed().toInt();
        acConnectedFile->close();

        if(nextAcConnected != acConnected) {
            acConnected = nextAcConnected;
            emit _acConnectedChanged(acConnected);
            logM(QString("AC: %1").arg(acConnected ? "connected" : "disconnected"));
        }
    }

    if(statusFile && statusFile->open(QIODevice::ReadOnly)) {
        QString nextState = (QString(statusFile->readLine().trimmed().toLower()));
        statusFile->close();

        if(nextState != state) {
            state = nextState;
            emit _stateChanged(state);
            logM("State: " + state);
        }
    }

    if(currentFile && currentFile->open(QIODevice::ReadOnly)) {
        int nextCurrent = currentFile->readLine().trimmed().toInt();
        currentFile->close();

        if(invertSign == 0) {
            bool connected = usbConnected || acConnected;
            if(connected && nextCurrent <= -200) {
                logL("Battery current inverted");
                invertSign = -1;
            }
            else if(connected && nextCurrent >= 200) {
                logL("Battery current not inverted");
                invertSign = 1;
            }
        }

        if (invertSign != 0) {
            nextCurrent = invertSign * nextCurrent;
        }

        if (current != nextCurrent) {
            current = nextCurrent;
            emit _currentChanged(current);
            logH(QString("Current: %1mA").arg(current / 1000));
        }
    }

    if(healthFile && healthFile->open(QIODevice::ReadOnly)) {
        QString nextHealth = (QString(healthFile->readLine().trimmed().toLower()));
        healthFile->close();

        if(nextHealth != health) {
            health = nextHealth;
            emit _healthChanged(health);
            logM("Health: " + health);
        }
    }

    if(temperatureFile && temperatureFile->open(QIODevice::ReadOnly)) {
        int nextTemperature = temperatureFile->readLine().trimmed().toInt() / tempCorrectionFactor;
        temperatureFile->close();

        if(nextTemperature != temperature) {
            temperature = nextTemperature;
            emit _temperatureChanged(temperature);
            logH(QString("Temperature: %1°C").arg(temperature / 10));
        }
    }

    if(timeToFullFile && timeToFullFile->open(QIODevice::ReadOnly)) {
        int nextTimeToFull = timeToFullFile->readLine().trimmed().toInt();
        timeToFullFile->close();

        if(nextTimeToFull != timeToFull) {
            // C2 reports 1 when not charging:
            timeToFull = (nextTimeToFull == 1) ? 0x7FFFFFFF : nextTimeToFull;
            emit _timeToFullChanged(timeToFull);
            logH(QString("Time to Full: %1s").arg(timeToFull));
        }
    }
}


int BatteryBase::getCharge()
{
    return charge;
}

int BatteryBase::getCurrent()
{
    return current;
}

int BatteryBase::getMaxChargeCurrent()
{
    return maxChargeCurrent;
}

QString BatteryBase::getState()
{
    return state;
}

QString BatteryBase::getHealth()
{
    return health;
}

int BatteryBase::getTemperature()
{
    return temperature;
}

bool BatteryBase::getChargingEnabled()
{
    return chargingEnabled;
}

bool BatteryBase::getUsbConnected()
{
    return usbConnected;
}

bool BatteryBase::getAcConnected()
{
    return acConnected;
}

int BatteryBase::getTimeToFull()
{
    return timeToFull;
}
