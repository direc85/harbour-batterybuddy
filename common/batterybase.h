/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2025 Matti Viljanen
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
#ifndef BATTERYBASE_H
#define BATTERYBASE_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include "logger.h"

class BatteryBase : public QObject
{
    Q_OBJECT

public:
    BatteryBase(Logger *newLogger, QObject *parent = nullptr);
    ~BatteryBase();

protected:
    QFile *acConnectedFile = nullptr;
    QFile *chargeFile = nullptr;
    QFile *controlFile = nullptr;
    QFile *currentFile = nullptr;
    QFile *healthFile = nullptr;
    QFile *maxCurrentFile = nullptr;
    QFile *statusFile = nullptr;
    QFile *temperatureFile = nullptr;
    QFile *usbConnectedFile = nullptr;

    Logger *logger = nullptr;

    // Number: 0 or 1
    const QStringList acConnectedFiles = {
        QStringLiteral("/sys/class/power_supply/ac/present"),
        QStringLiteral("/sys/class/power_supply/axp813-ac/present")};

    // Battery charge percentage, number, e.g. 42
    const QStringList chargeFiles = {
        QStringLiteral("/sys/class/power_supply/battery/capacity"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/capacity"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/capacity")};

    // Charger control file
    const QStringList controlFiles = {
        // e.g. Sony Xperia XA2
        QStringLiteral("/sys/class/power_supply/battery/input_suspend"),
        // e.g. for Sony Xperia Z3 Compact Tablet
        QStringLiteral("/sys/class/power_supply/battery/charging_enabled"),
        // e.g. for Jolla Phone
        QStringLiteral("/sys/class/power_supply/usb/charger_disable"),
        // e.g. for Jolla Tablet
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/enable_charging"),
        // e.g. Jolla C2
        QStringLiteral("/sys/class/power_supply/battery/charger.0/stop_charge")
    };

    // Charging/discharging current in microamps, e.g. -1450000 (-145mA)
    const QStringList currentFiles = {
        QStringLiteral("/sys/class/power_supply/battery/current_now"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/current_now"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/current_now")};

    // String: health state
    const QStringList healthFiles = {
        QStringLiteral("/sys/class/power_supply/battery/health"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/health"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/health")};

    // Maximum charge current in microamps, e.g. 3500000 (3500mA)
    const QStringList maxCurrentFiles = {
        QStringLiteral("/sys/class/power_supply/battery/constant_charge_current_max"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/constant_charge_current_max")};

    // Battery/charging status: charging, discharging, full, empty, unknown (others?)
    const QStringList statusFiles = {
        QStringLiteral("/sys/class/power_supply/battery/status"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/status"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/status")};

    // Number: temperature
    const QStringList temperatureFiles = {
        QStringLiteral("/sys/class/power_supply/battery/temp"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/temp"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/hwmon0/in0_input")};

    // Number: 0 or 1
    const QStringList usbConnectedFiles = {
        QStringLiteral("/sys/class/power_supply/usb/present"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_charger/present"),
        QStringLiteral("/sys/class/power_supply/axp20x-usb/present"),
        QStringLiteral("/sys/class/power_supply/usb/online")};

    // Default values:
    int charge = 100;              // Charge percentage, 0..100
    int current = 0;               // Charging/discharging current in microamps
    bool usbConnected = false;     // USB charger plugged in
    bool acConnected = false;      // AC plugged in
    QString state = "idle";        // Charger/battery status string: charging, disaharging, not charging, idle, unknown
    bool chargingEnabled = true;   // Is charging currently enabled or disabled by the service
    int maxChargeCurrent = 0;      // Charge current limit in microamps

    QString health = "unknown";       // Battery health string: hot, warm, good, cool, cold, overheat, overvoltage
    int temperature = 0x7FFFFFFF;     // Temperature in centi-celcius. INT32_MAX means "unknown"
    float tempCorrectionFactor = 1.0; // PineTab outputs an integer in centi-centigrade

    int enableChargingValue = 0;
    int disableChargingValue = 1;
    bool chargerIsEnabled = true;

    int nextCharge = charge;
    int invertSign = 1;
    bool invertDecided = false;

    bool nextUsbConnected = false;
    bool nextAcConnected = false;
    QString nextState = "idle";
    bool nextChargingEnabled = false;

    QString nextHealth = health;
    int nextTemperature = temperature;

    int maxSupportedCurrent = -1;

    int getCharge();
    int getCurrent();
    int getMaxChargeCurrent();
    bool getCharging();
    bool getUsbConnected();
    bool getAcConnected();
    QString getState();

    QString getHealth();
    int getTemperature();

    bool getChargingEnabled();

    void updateBaseData();

signals:
    void _chargeChanged(int);
    void _currentChanged(int);
    void _stateChanged(QString);
    void _chargingEnabledChanged(bool);
    void _chargerConnectedChanged(bool);
    void _acConnectedChanged(bool);
    void _healthChanged(QString);
    void _temperatureChanged(int);
};

#endif // BATTERYBASE_H
