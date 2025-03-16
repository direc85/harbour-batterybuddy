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

Battery::Battery(Settings* newSettings, Logger* newLogger, QObject* parent)
: BatteryBase(newLogger, parent) {
    settings = newSettings;

    connect(this, SIGNAL(_chargeChanged(int)), this, SIGNAL(chargeChanged(int)));
    connect(this, SIGNAL(_currentChanged(int)), this, SIGNAL(currentChanged(int)));
    connect(this, SIGNAL(_stateChanged(QString)), this, SIGNAL(stateChanged(QString)));
    connect(this, SIGNAL(_chargingEnabledChanged(bool)), this, SIGNAL(chargingEnabledChanged(bool)));
    connect(this, SIGNAL(_chargerConnectedChanged(bool)), this, SIGNAL(chargerConnectedChanged(bool)));
    connect(this, SIGNAL(_acConnectedChanged(bool)), this, SIGNAL(acConnectedChanged(bool)));
    connect(this, SIGNAL(_healthChanged(QString)), this, SIGNAL(healthChanged(QString)));
    connect(this, SIGNAL(_temperatureChanged(int)), this, SIGNAL(temperatureChanged(int)));

    updateData();
}

void Battery::updateData() {
    updateBaseData();
}