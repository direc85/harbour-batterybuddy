// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021-2023 Peter G. <sailfish@nephros.org>

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

    bool setChargingEnabled(const bool isEnabled);
    void enableRestrictCharging(const bool enabled);

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
    void onUpdateTimerRunning();
    void onHighNotifyTimerRunning();
    void onLowNotifyTimerRunning();
    void onHealthNotifyTimerRunning();
};

#endif // SVC_BATTERY_H
