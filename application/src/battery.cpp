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
    : BatteryBase(newLogger, parent)
{
    settings = newSettings;

    connect(this, &BatteryBase::_chargeChanged, this, &Battery::chargeChanged);
    connect(this, &BatteryBase::_currentChanged, this, &Battery::currentChanged);
    connect(this, &BatteryBase::_stateChanged, this, &Battery::stateChanged);
    connect(this, &BatteryBase::_chargingEnabledChanged, this, &Battery::chargingEnabledChanged);
    connect(this, &BatteryBase::_chargerConnectedChanged, this, &Battery::chargerConnectedChanged);
    connect(this, &BatteryBase::_acConnectedChanged, this, &Battery::acConnectedChanged);
    connect(this, &BatteryBase::_healthChanged, this, &Battery::healthChanged);
    connect(this, &BatteryBase::_temperatureChanged, this, &Battery::temperatureChanged);
    connect(this, &BatteryBase::_timeToFullChanged, this, &Battery::timeToFullChanged);

    updateData();
}

void Battery::updateData() {
    updateBaseData();
}
