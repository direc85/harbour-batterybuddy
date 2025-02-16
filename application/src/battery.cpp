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

    BatteryBase* base = (BatteryBase *)parent;
    connect(base, &BatteryBase::chargeChanged, this, &Battery::chargeChanged);
    connect(base, &BatteryBase::currentChanged, this, &Battery::currentChanged);
    connect(base, &BatteryBase::stateChanged, this, &Battery::stateChanged);
    connect(base, &BatteryBase::chargingEnabledChanged, this, &Battery::chargingEnabledChanged);
    connect(base, &BatteryBase::chargerConnectedChanged, this, &Battery::chargerConnectedChanged);
    connect(base, &BatteryBase::acConnectedChanged, this, &Battery::acConnectedChanged);
    connect(base, &BatteryBase::healthChanged, this, &Battery::healthChanged);
    connect(base, &BatteryBase::temperatureChanged, this, &Battery::temperatureChanged);
}

Battery::~Battery() { }
