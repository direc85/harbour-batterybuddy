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
 * General Public License along with Battery Buddy. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QHostInfo>
#include <QDebug>
#include "settings.h"
#include "notification.h"

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int charge READ getCharge NOTIFY chargeChanged)
    Q_PROPERTY(bool chargerConnected READ getChargerConnected NOTIFY chargerConnectedChanged)
    Q_PROPERTY(QString state READ getState NOTIFY stateChanged)
    Q_PROPERTY(bool chargingEnabled READ getChargingEnabled  WRITE setChargingEnabled NOTIFY chargingEnabledChanged)

public:
    Battery(Settings *newSettings, QTimer *newUpdater, QTimer *newNotifier, Notification *newNotification, QObject *parent = nullptr);
    ~Battery();

    int getCharge();
    bool getCharging();
    bool getChargerConnected();
    QString getState();

    bool getChargingEnabled();
    void setChargingEnabled(bool);

public slots:
    void updateData();
    void shutdown();

private:
    QFile *chargeFile = nullptr;
    QFile *chargerConnectedFile = nullptr;
    QFile *stateFile = nullptr;
    QFile *chargingEnabledFile = nullptr;
    Settings *settings = nullptr;
    QTimer *updateTimer = nullptr;
    QTimer *notifyTimer = nullptr;
    Notification *notification = nullptr;

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

    QFileDevice::Permissions originalPerms; // Updated in constructor
    QFileDevice::Permissions customPerms = static_cast<QFileDevice::Permissions>(0x0666);

signals:
    void chargeChanged(int);
    void stateChanged(QString);
    void chargingEnabledChanged(bool);
    void chargerConnectedChanged(bool);

public slots:
    void updateConfig();
    void showNotification();
};

#endif // BATTERY_H
