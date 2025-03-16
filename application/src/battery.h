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
