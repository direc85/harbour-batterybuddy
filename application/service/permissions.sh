#!/bin/sh

# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: 2026 Matti Viljanen <matti.viljanen@kapsi.fi>

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
