#!/bin/sh

# Battery Buddy, a Sailfish application to prolong battery lifetime
#
# Copyright (C) 2019-2020 Matti Viljanen
#
# Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
# GNU General Public License as published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU General Public License for more details. You should have received a copy of the GNU
# General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
#
# Author: Matti Viljanen

chmod 644 /sys/class/power_supply/battery/input_suspend 2>/dev/null
chmod 644 /sys/class/power_supply/battery/charging_enabled 2>/dev/null
chmod 644 /sys/class/power_supply/usb/charger_disable 2>/dev/null
chmod 644 /sys/class/power_supply/dollar_cove_battery/enable_charging 2>/dev/null
exit 0
