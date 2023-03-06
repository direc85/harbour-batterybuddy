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
#include "settings.h"
#include "logger.h"

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int charge READ getCharge NOTIFY chargeChanged)
    Q_PROPERTY(int current READ getCurrent NOTIFY currentChanged)
    Q_PROPERTY(int maxChargeCurrent READ getMaxChargeCurrent)
    Q_PROPERTY(bool chargerConnected READ getChargerConnected NOTIFY chargerConnectedChanged)
    Q_PROPERTY(bool acConnected READ getAcConnected NOTIFY acConnectedChanged)
    Q_PROPERTY(QString state READ getState NOTIFY stateChanged)
    Q_PROPERTY(bool chargingEnabled READ getChargingEnabled NOTIFY chargingEnabledChanged)

    Q_PROPERTY(QString health READ getHealth NOTIFY healthChanged)
    Q_PROPERTY(int temperature READ getTemperature NOTIFY temperatureChanged)

public:
    Battery(Settings* newSettings, Logger* newLogger, QObject* parent = nullptr);
    ~Battery();

    int getCharge();
    int getCurrent();
    int getMaxChargeCurrent();
    bool getCharging();
    bool getChargerConnected();
    bool getAcConnected();
    QString getState();

    QString getHealth();
    int getTemperature();

    bool getChargingEnabled();

public slots:
    void updateData();

private:
    QFile* chargeFile = nullptr;
    QFile* currentFile = nullptr;
    QFile* chargerConnectedFile = nullptr;
    QFile* acConnectedFile = nullptr;
    QFile* stateFile = nullptr;
    QFile* chargingEnabledFile = nullptr;
    QFile* maxChargeCurrentFile = nullptr;
    Settings* settings = nullptr;
    Logger* logger = nullptr;

    QFile* temperatureFile = nullptr;
    QFile* healthFile = nullptr;

    // Default values:
    int charge = 100; // 100% full
    int current = 0; // Not charging/discharging
    bool chargerConnected = false; // Charger plugged in
    bool acConnected = false; // AC plugged in
    QString state = "idle"; // dis/charging, idle, unknown
    bool chargingEnabled = true; // Only ever disabled manually
    int maxChargeCurrent = 0; // Charge current limit in micro amps

    QString health = "unknown"; // Good, warm, overheat. Might have Cold or Overvoltage depending on driver
    int temperature = 0x7FFFFFFF; // This value means "unknown" (32-bit INT_MAX)
    float tempCorrectionFactor = 1.0; // PineTab outputs an integer in centi-centigrade

    int enableChargingValue = 1;
    int disableChargingValue = 0;
    bool chargerIsEnabled = true;

    int nextCharge = charge;
    bool invertCurrent = false;
    bool invertDecided = false;

    bool nextChargerConnected = chargerConnected;
    bool nextAcConnected = acConnected;
    QString nextState = state;
    bool nextChargingEnabled = chargingEnabled;

    QString  nextHealth = health;
    int nextTemperature = temperature;

signals:
    void chargeChanged(int);
    void currentChanged(int);
    void stateChanged(QString);
    void chargingEnabledChanged(bool);
    void chargerConnectedChanged(bool);
    void acConnectedChanged(bool);
    void healthChanged(QString);
    void temperatureChanged(int);
};

#endif // BATTERY_H
