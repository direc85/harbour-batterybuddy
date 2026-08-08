#!/bin/sh

# Battery Buddy, a Sailfish application to prolong battery lifetime
#
# Copyright (C) 2019-2026 Matti Viljanen
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

if [ "$1" = "enable" ]; then
    PERMS="666"
elif [ "$1" = "disable" ]; then
    PERMS="644"
else
    echo "Usage: $0 [enable|disable]"
    exit 1
fi

while read FILE; do
    [ -f "$FILE" ] && chmod "$PERMS" "$FILE"
done <<EOF
/sys/class/power_supply/battery/input_suspend
/sys/class/power_supply/battery/charging_enabled
/sys/class/power_supply/battery/constant_charge_current_max
/sys/class/power_supply/usb/charger_disable
/sys/class/power_supply/dollar_cove_battery/enable_charging
/sys/class/power_supply/axp-20x-battery/constant_charge_current_max
/sys/class/power_supply/battery/charger.0/stop_charge
/sys/class/power_supply/primary_chg/online
/sys/class/power_supply/mtk-master-charger/constant_charge_current_max
/sys/class/qcom-battery/charging_enabled
/sys/class/qcom-battery/restrict_cur
/sys/class/qcom-battery/restrict_chg
EOF

exit 0
