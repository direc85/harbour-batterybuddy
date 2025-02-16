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
#ifndef SVC_BATTERY_H
#define SVC_BATTERY_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include <QLocale>
#include <QCoreApplication>
#include <keepalive/backgroundactivity.h>
#include "batterybase.h"
#include "settings.h"
#include "mynotification.h"
#include "logger.h"

class Battery : public BatteryBase
{
    Q_OBJECT

public:
    Battery(Settings* newSettings, Logger* newLogger, QCoreApplication *app, QObject *parent = nullptr);
    ~Battery();

    int getCharge();
    bool getCharging();
    bool getUsbConnected();
    bool getAcConnected();
    QString getState();

    bool getChargingEnabled();
    bool setChargingEnabled(const bool isEnabled);

    int getTemperature();
    QString getHealth();

public slots:
    void updateData();
    void shutdown();

private:
    bool unclosedChargeNotification = false;
    bool unclosedHealthNotification = false;

    BackgroundActivity::Frequency frequencies[14] = {
        BackgroundActivity::Range,
        BackgroundActivity::ThirtySeconds,
        BackgroundActivity::TwoAndHalfMinutes,
        BackgroundActivity::FiveMinutes,
        BackgroundActivity::TenMinutes,
        BackgroundActivity::FifteenMinutes,
        BackgroundActivity::ThirtyMinutes,
        BackgroundActivity::OneHour,
        BackgroundActivity::TwoHours,
        BackgroundActivity::FourHours,
        BackgroundActivity::EightHours,
        BackgroundActivity::TenHours,
        BackgroundActivity::TwelveHours,
        BackgroundActivity::TwentyFourHours
    };

    Settings *settings = nullptr;

    BackgroundActivity *updateTimer = nullptr;
    BackgroundActivity *highNotifyTimer = nullptr;
    BackgroundActivity *lowNotifyTimer = nullptr;
    BackgroundActivity *healthNotifyTimer = nullptr;

    MyNotification *chargeNotification = nullptr;
    MyNotification *healthNotification = nullptr;

    QFileDevice::Permissions originalPerms; // Updated in constructor
    QFileDevice::Permissions customPerms = static_cast<QFileDevice::Permissions>(0x0666);

signals:
    void chargeChanged(int);
    void currentChanged(int);
    void stateChanged(QString);
    void chargingEnabledChanged(bool);
    void chargerConnectedChanged(bool);
    void acConnectedChanged(bool);
    void healthChanged(QString);
    void temperatureChanged(int);

public slots:
    void resetTimers();
    void showHighNotification();
    void showLowNotification();
    void showHealthNotification();
    void setMaxChargeCurrent(int newCurrent);

private slots:
    void healthHandler(QString);
    void stateHandler(QString);
};

#endif // SVC_BATTERY_H
