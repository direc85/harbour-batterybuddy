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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int     lowerLimit     READ getLowerLimit   WRITE setLowerLimit   NOTIFY lowerLimitChanged)
    Q_PROPERTY(int     upperLimit     READ getUpperLimit   WRITE setUpperLimit   NOTIFY upperLimitChanged)
    Q_PROPERTY(int     interval       READ getInterval     WRITE setInterval     NOTIFY intervalChanged)
    Q_PROPERTY(bool    limitEnabled   READ getLimitEnabled WRITE setLimitEnabled NOTIFY limitEnabledChanged)
    Q_PROPERTY(int     chargeLimit    READ getChargeLimit  WRITE setChargeLimit  NOTIFY chargeLimitChanged)
    Q_PROPERTY(QString lowAlertFile   READ getLowAlert                           NOTIFY lowAlertChanged)
    Q_PROPERTY(QString highAlertFile  READ getHighAlert                          NOTIFY highAlertChanged)

public:
    Settings(QObject* parent = nullptr);
    ~Settings();

    // QML-exposed stuff
    int getLowerLimit();
    int getUpperLimit();
    int getInterval();
    bool getLimitEnabled();
    int getChargeLimit();
    QString getLowAlert();
    QString getHighAlert();
    void setLowerLimit(int newLimit);
    void setUpperLimit(int newLimit);
    void setInterval(int newInterval);
    void setLimitEnabled(bool newEnabled);
    void setChargeLimit(int newLimit);

private:
    QSettings mySettings;

    // Default values
    int lowerLimit = 25;
    int upperLimit = 75;
    int interval = 60;
    bool limitEnabled = false;
    int chargeLimit = 70;
    QString lowAlertFile = "/usr/share/sounds/jolla-ambient/stereo/general_warning.wav";
    QString highAlertFile = "/usr/share/sounds/jolla-ambient/stereo/positive_confirmation.wav";

signals:
    int lowerLimitChanged();
    int upperLimitChanged();
    int intervalChanged();
    bool limitEnabledChanged();
    int chargeLimitChanged();
    QString lowAlertChanged();
    QString highAlertChanged();
};

#endif // SETTINGS_H
