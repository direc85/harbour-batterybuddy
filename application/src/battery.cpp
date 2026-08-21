// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021-2025 Peter G. <sailfish@nephros.org>

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
