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

#ifndef COMMON_DEVICES_H
#define COMMON_DEVICES_H

#include <QString>
#include <QStringList>

    // Battery charge percentage, number, e.g. 42
    const QStringList capacityFiles = {
        "/sys/class/power_supply/battery/capacity",
        "/sys/class/power_supply/dollar_cove_battery/capacity",
        "/sys/class/power_supply/axp20x-battery/capacity"
    };

    // Charging/discharging current in microamps, e.g. -1450000 (-145mA)
    const QStringList currentFiles = {
        "/sys/class/power_supply/battery/current_now",
        "/sys/class/power_supply/dollar_cove_battery/current_now",
        "/sys/class/power_supply/axp20x-battery/current_now"
    };

    // Maximum charge current in microamps, e.g. 3500000 (3500mA)
    const QStringList maxCurrentFiles = {
        "/sys/class/power_supply/battery/constant_charge_current_max",
        "/sys/class/power_supply/axp20x-battery/constant_charge_current_max"
    };

    // Battery/charging status: charging, discharging, full, empty, unknown (others?)
    const QStringList statusFiles = {
        "/sys/class/power_supply/battery/status",
        "/sys/class/power_supply/dollar_cove_battery/status",
        "/sys/class/power_supply/axp20x-battery/status"
    };

    // Charger connected, bool (number): 0 or 1
    const QStringList chargerFiles = {
        "/sys/class/power_supply/usb/present",
        "/sys/class/power_supply/dollar_cove_charger/present",
        "/sys/class/power_supply/axp20x-usb/present"
    };

    // Charger connected, bool (number): 0 or 1
    const QStringList acFiles = {
        "/sys/class/power_supply/ac/present",
        "/sys/class/power_supply/axp813-ac/present"
    };

    // Number: temperature
    const QStringList tempFiles = {
        "/sys/class/power_supply/battery/temp",
        "/sys/class/power_supply/dollar_cove_battery/temp",
        "/sys/class/power_supply/axp20x-battery/hwmon0/in0_input"
    };

    // String: health state
    const QStringList healthFiles = {
        "/sys/class/power_supply/battery/health",
        "/sys/class/power_supply/dollar_cove_battery/health",
        "/sys/class/power_supply/axp20x-battery/health"
    };

    // Charger control file
    const QStringList controlFiles = {
        "/sys/class/power_supply/battery/input_suspend",              // e.g. Sony Xperia XA2
        "/sys/class/power_supply/battery/charging_enabled",           // e.g. for Sony Xperia Z3 Compact Tablet
        "/sys/class/power_supply/usb/charger_disable",                // e.g. for Jolla Phone
        "/sys/class/power_supply/dollar_cove_battery/enable_charging" // e.g. for Jolla Tablet
    };


#endif  //COMMON_DEVICES_H
