// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2025 Peter G. <sailfish@nephros.org>

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
    QFile *restrictCurrentFile = nullptr;
    QFile *statusFile = nullptr;
    QFile *temperatureFile = nullptr;
    QFile *usbConnectedFile = nullptr;
    QFile *timeToFullFile = nullptr;

    Logger *logger = nullptr;

    // Number: 0 or 1
    const QStringList acConnectedFiles = {
        QStringLiteral("/sys/class/power_supply/ac/present"),
        QStringLiteral("/sys/class/power_supply/axp813-ac/present"),
    };

    // Battery charge percentage, number, e.g. 42
    const QStringList chargeFiles = {
        QStringLiteral("/sys/class/power_supply/battery/capacity"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/capacity"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/capacity"),
    };

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
        QStringLiteral("/sys/class/power_supply/battery/charger.0/stop_charge"),
        // e.g. Jolla Phone
        QStringLiteral("/sys/class/power_supply/primary_chg/bypass_mode"),
        // e.g. Sony Xperia 1 IV, 5 IV
        QStringLiteral("/sys/class/qcom-battery/charging_enabled"),
    };

    // Charging/discharging current in microamps, e.g. -1450000 (-145mA)
    const QStringList currentFiles = {
        QStringLiteral("/sys/class/power_supply/battery/current_now"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/current_now"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/current_now"),
    };

    // String: health state
    const QStringList healthFiles = {
        QStringLiteral("/sys/class/power_supply/battery/health"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/health"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/health"),
    };

    // Maximum charge current in microamps, e.g. 3500000 (3500mA)
    const QStringList maxCurrentFiles = {
        QStringLiteral("/sys/class/power_supply/battery/constant_charge_current_max"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/constant_charge_current_max"),
        QStringLiteral("/sys/class/qcom-battery/restrict_cur"),
        // Defaults to -1 -- unsupported default value
        QStringLiteral("/sys/class/power_supply/mtk-master-charger/constant_charge_current_max_DISABLED"),
    };

    // Enable or disable restricting current files, 0 or 1
    const QStringList restrictCurrentFiles = {
        QStringLiteral("/sys/class/qcom-battery/restrict_chg"),
    };

    // Battery/charging status: charging, discharging, full, empty, unknown (others?)
    const QStringList statusFiles = {
        QStringLiteral("/sys/class/power_supply/battery/status"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/status"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/status"),
    };

    // Number: temperature
    const QStringList temperatureFiles = {
        QStringLiteral("/sys/class/power_supply/battery/temp"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_battery/temp"),
        QStringLiteral("/sys/class/power_supply/axp20x-battery/hwmon0/in0_input"),
    };

    // Number: 0 or 1
    const QStringList usbConnectedFiles = {
        QStringLiteral("/sys/class/power_supply/usb/present"),
        QStringLiteral("/sys/class/power_supply/dollar_cove_charger/present"),
        QStringLiteral("/sys/class/power_supply/axp20x-usb/present"),
        QStringLiteral("/sys/class/power_supply/usb/online"),
        QStringLiteral("/sys/class/power_supply/mtk-master-charger/online"),
    };

    // Number: Time to full charge, in seconds
    const QStringList timeToFullFiles = {
        // e.g. Jolla C2, Jolla Phone
        QStringLiteral("/sys/class/power_supply/battery/time_to_full_now"),
    };


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

    int invertSign = 0; // 0 = undecided, 1 = keep, -1 = flip

    int maxSupportedCurrent = -1;

    int timeToFull = 0x7FFFFFFF;     // Seconds to full charge. INT32_MAX means "unknown"

    int getCharge();
    int getInvertSign();
    int getCurrent();
    int getMaxChargeCurrent();
    bool getCharging();
    bool getUsbConnected();
    bool getAcConnected();
    QString getState();
    QString getHealth();
    int getTemperature();
    int getTimeToFull();
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
    void _timeToFullChanged(int);
    void _invertSignChanged(int);
};

#endif // BATTERYBASE_H
