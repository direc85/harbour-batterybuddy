#!/bin/bash
chmod 644 /sys/class/power_supply/battery/input_suspend 2>/dev/null
chmod 644 /sys/class/power_supply/battery/charging_enabled 2>/dev/null
chmod 644 /sys/class/power_supply/usb/charger_disable 2>/dev/null
exit 0
