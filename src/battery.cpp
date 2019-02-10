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

Battery::Battery(QObject* parent) : QObject(parent)
{
    // Number: meaning percentage, e.g. 42
    chargeFile   = new QFile("/run/state/namespaces/Battery/ChargePercentage", this);
    // Number: 0 or 1
    chargingFile = new QFile("/run/state/namespaces/Battery/IsCharging", this);
    // String: charging, discharging, (empty), unknown (others?)
    stateFile   = new QFile("/run/state/namespaces/Battery/ChargingState", this);

    // TODO
    // What if the files can't be opened?
    // What if the device doesn't have a battery?

    updateData();
}

Battery::~Battery() { }

void Battery::updateData()
{
    if(chargeFile->open(QIODevice::ReadOnly)) {
        nextCharge = chargeFile->readAll().toInt();
        if(nextCharge != charge) {
            charge = nextCharge;
            emit chargeChanged();
        }
        chargeFile->close();
    }
    if(chargingFile->open(QIODevice::ReadOnly)) {
        nextCharging = (chargingFile->readAll().toInt() == 0 ? false : true);
        if(nextCharging != charging) {
            charging = nextCharging;
            emit chargingChanged();
        }
        chargingFile->close();
    }
    if(stateFile->open(QIODevice::ReadOnly)) {
        nextState = (QString(stateFile->readAll()));
        if(nextState != state) {
            state = nextState;
            emit stateChanged();
        }
        stateFile->close();
    }
}

int Battery::getCharge(){ return charge; }

bool Battery::getCharging() { return charging; }

QString Battery::getState() { return state; }
