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

class Battery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int     charge    READ getCharge    NOTIFY chargeChanged  )
    Q_PROPERTY(bool    charging  READ getCharging  NOTIFY chargingChanged)
    Q_PROPERTY(QString state     READ getState     NOTIFY stateChanged)

public:
    Battery(QObject* parent = nullptr);
    ~Battery();

    int getCharge();
    bool getCharging();
    QString getState();

public slots:
    void updateData();

private:
    QFile* chargeFile;
    QFile* chargingFile;
    QFile* stateFile;

    // Default values:
    int charge    = 100;     // 100% full
    bool charging = true;    // Charger plugged in
    QString state = "idle";  // Not charging
    QString stateTr = state; // Translated state text

    int nextCharge     = charge;
    bool nextCharging  = charging;
    QString nextStatus = state;

signals:
    int chargeChanged();
    bool chargingChanged();
    QString stateChanged();
};

#endif // BATTERY_H
