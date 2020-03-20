/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#include "battery.h"

Battery::Battery(Settings* newSettings, QObject* parent) : QObject(parent)
{
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
        qInfo() << "Sailfish SDK detected";
        qInfo() << "Charger control feature disabled";
    }
    else {
    chargingEnabledFile = new QFile(this);

    // e.g. for Sony Xperia XA2
    chargingEnabledFile->setFileName("/sys/class/power_supply/battery/input_suspend");
    if(chargingEnabledFile->exists()) {
        enableChargingValue = 0;
        disableChargingValue = 1;
    }

    // e.g. for Sony Xperia Z3 Compact Tablet
    chargingEnabledFile->setFileName("/sys/class/power_supply/battery/charging_enabled");
    if(chargingEnabledFile->exists()) {
        enableChargingValue = 1;
        disableChargingValue = 0;
    }

    // e.g. for Jolla Phone
    chargingEnabledFile->setFileName("/sys/class/power_supply/usb/charger_disable");
    if(chargingEnabledFile->exists()) {
        enableChargingValue = 0;
        disableChargingValue = 1;
    }
    else {
        delete chargingEnabledFile;
        chargingEnabledFile = Q_NULLPTR;
        qWarning() << "Charger control file not found!";
        qWarning() << "Please contact the developer with your device model!";
    }

    if(chargingEnabledFile) {
        if(chargingEnabledFile->open(QIODevice::WriteOnly)) {
            qInfo() << "Controlling charging via" << chargingEnabledFile->fileName();
            chargingEnabledFile->close();
        }
        else {
            delete chargingEnabledFile;
            chargingEnabledFile = Q_NULLPTR;
            qWarning() << "Charger control file" << chargingEnabledFile->fileName() << "is not writable";
            qWarning() << "Charger control feature disabled";
        }
        }
    }

    // TODO
    // Implement DBus mechanism for reading battery status, or try
    // QFileSystemWatcher again without /run/state/namespaces/Battery/
    // thingamabob - it is deprecated anyway.

    updateData();
}

Battery::~Battery() { }

void Battery::updateData()
{
    if(chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readLine().trimmed().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            emit chargeChanged();
            qDebug() << "Battery:" << charge;
        }
        chargeFile->close();
    }
    if(chargerConnectedFile->open(QIODevice::ReadOnly)) {
        nextChargerConnected = chargerConnectedFile->readLine().trimmed().toInt();
        if(nextChargerConnected != chargerConnected) {
            chargerConnected = nextChargerConnected;
            emit chargerConnectedChanged();
            qDebug() << "Charger is connected:" << chargerConnected;
        }
        chargerConnectedFile->close();
    }
    if(stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readLine().trimmed().toLower()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged();
            qDebug() << "Charging status:" << state;
        }
        stateFile->close();
    }
    // This can't be used, because on Jolla Phone the file always reads "0" :(
    // It doesn't matter that much anyway, because the value changes only when we change it.

    // if(chargingEnabledFile && chargingEnabledFile->open(QIODevice::ReadOnly)) {
    //     nextChargingEnabled = chargingEnabledFile->readLine().trimmed().toInt() == enableChargingValue;
    //     if(nextChargingEnabled != chargingEnabled) {
    //         chargingEnabled = nextChargingEnabled;
    //         emit chargingEnabledChanged();
    //     }
    //     chargingEnabledFile->close();
    // }

    if(chargingEnabledFile && settings->getLimitEnabled()) {
        if(chargingEnabled && charge >= settings->getHighLimit()) {
            setChargingEnabled(false);
        }
        else if(!chargingEnabled && charge <= settings->getLowLimit()) {
            setChargingEnabled(true);
        }
    }
}

int Battery::getCharge(){ return charge; }

QString Battery::getState() { return state; }

bool Battery::getChargingEnabled() { return chargingEnabled; }

void Battery::setChargingEnabled(bool isEnabled) {
    if(chargingEnabledFile && chargingEnabledFile->open(QIODevice::WriteOnly)) {
        if(chargingEnabledFile->write(QString("%1").arg(isEnabled ? enableChargingValue : disableChargingValue).toLatin1())) {
            chargingEnabled = isEnabled;
            emit chargingEnabledChanged();

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
