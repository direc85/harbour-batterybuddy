#ifndef BATTERY_H
#define BATTERY_H
/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */

#include <QObject>
#include <QString>
#include <QFile>
#include <QDebug>
#include "settings.h"

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int charge READ getCharge NOTIFY chargeChanged)
    Q_PROPERTY(bool chargerConnected READ getChargerConnected NOTIFY chargerConnectedChanged)
    Q_PROPERTY(QString state READ getState NOTIFY stateChanged)
    Q_PROPERTY(bool chargingEnabled READ getChargingEnabled  WRITE setChargingEnabled NOTIFY chargingEnabledChanged)

public:
    Battery(Settings* newSettings, QObject* parent = nullptr);
    ~Battery();

    int getCharge();
    bool getCharging();
    bool getChargerConnected();
    QString getState();

    bool getChargingEnabled();
    void setChargingEnabled(bool);

public slots:
    void updateData();

private:
    QFile* chargeFile;
    QFile* chargerConnectedFile;
    QFile* stateFile;
    QFile* chargingEnabledFile = Q_NULLPTR;
    Settings* settings;

    // Default values:
    int charge = 100; // 100% full
    bool chargerConnected = false; // Charger plugged in
    QString state = "idle"; // dis/charging, idle, unknown
    bool chargingEnabled = true; // Only ever disabled manually

    int enableChargingValue = 1;
    int disableChargingValue = 0;
    bool chargerIsEnabled = true;

    int nextCharge = charge;
    bool nextChargerConnected = chargerConnected;
    QString nextState = state;
    bool nextChargingEnabled = chargingEnabled;

signals:
    int chargeChanged();
    QString stateChanged();
    bool chargingEnabledChanged();
    bool chargerConnectedChanged();
};

#endif // BATTERY_H
