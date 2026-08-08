// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021-2025 Peter G. <sailfish@nephros.org>

#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QSysInfo>
#include "batterybase.h"
#include "settings.h"
#include "logger.h"

class Battery : public BatteryBase
{
    Q_OBJECT
    Q_PROPERTY(int charge READ getCharge NOTIFY chargeChanged)
    Q_PROPERTY(int current READ getCurrent NOTIFY currentChanged)
    Q_PROPERTY(int maxChargeCurrent READ getMaxChargeCurrent)
    Q_PROPERTY(bool chargerConnected READ getUsbConnected NOTIFY chargerConnectedChanged)
    Q_PROPERTY(bool acConnected READ getAcConnected NOTIFY acConnectedChanged)
    Q_PROPERTY(QString state READ getState NOTIFY stateChanged)
    Q_PROPERTY(bool chargingEnabled READ getChargingEnabled NOTIFY chargingEnabledChanged)

    Q_PROPERTY(QString health READ getHealth NOTIFY healthChanged)
    Q_PROPERTY(int temperature READ getTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(int timeToFull READ getTimeToFull NOTIFY timeToFullChanged)

public:
    Battery(Settings* newSettings, Logger* newLogger, QObject* parent = nullptr);

public slots:
    void updateData();

private:
    Settings* settings;

signals:
    void chargeChanged(int);
    void currentChanged(int);
    void stateChanged(QString);
    void chargingEnabledChanged(bool);
    void chargerConnectedChanged(bool);
    void acConnectedChanged(bool);
    void healthChanged(QString);
    void temperatureChanged(int);
    void timeToFullChanged(int);
};

#endif // BATTERY_H
