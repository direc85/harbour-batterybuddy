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
#include <QDebug>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int lowAlert READ getLowAlert WRITE setLowAlert NOTIFY lowAlertChanged)
    Q_PROPERTY(int highAlert READ getHighAlert WRITE setHighAlert NOTIFY highAlertChanged)
    Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(int highLimit READ getHighLimit WRITE setHighLimit NOTIFY highLimitChanged)
    Q_PROPERTY(int lowLimit READ getLowLimit WRITE setLowLimit NOTIFY lowLimitChanged)
    Q_PROPERTY(bool limitEnabled READ getLimitEnabled WRITE setLimitEnabled NOTIFY limitEnabledChanged)
    Q_PROPERTY(bool notificationsEnabled READ getNotificationsEnabled WRITE setNotificationsEnabled NOTIFY notificationsEnabledChanged)
    Q_PROPERTY(QString lowAlertFile READ getLowAlertFile NOTIFY lowAlertFileChanged)
    Q_PROPERTY(QString highAlertFile READ getHighAlertFile NOTIFY highAlertFileChanged)

public:
    Settings(QObject* parent = nullptr);
    ~Settings();

    int  getLowAlert() { return lowAlert; }
    int  getHighAlert() { return highAlert; }
    int  getInterval() { return interval; }
    int  getLowLimit() { return lowLimit; }
    int  getHighLimit() { return highLimit; }
    bool getLimitEnabled() { return limitEnabled == 1; }
    bool getNotificationsEnabled() { return notificationsEnabled == 1; }
    QString getLowAlertFile() { return lowAlertFile; }
    QString getHighAlertFile() { return highAlertFile;}

    void setLowAlert(int newLimit) { lowAlert = newLimit; emit lowAlertChanged(); }
    void setHighAlert(int newLimit) { highAlert = newLimit; emit highAlertChanged(); }
    void setInterval(int newInterval) { interval = newInterval; emit intervalChanged(); }
    void setLowLimit(int newLimit) { lowLimit = newLimit; emit lowLimitChanged(); }
    void setHighLimit(int newLimit) { highLimit = newLimit; emit highLimitChanged(); }
    void setLimitEnabled(bool newEnabled) { limitEnabled = (newEnabled ? 1 : 0); emit limitEnabledChanged(); }
    void setNotificationsEnabled(bool newEnabled) { notificationsEnabled = (newEnabled ? 1 : 0); emit notificationsEnabledChanged(); }


private:
    QSettings mySettings;

    // Default values
    int lowAlert = 25;
    int highAlert = 75;
    int interval = 60;
    int limitEnabled = 0; // Converted to boolean for QML
    int notificationsEnabled = 1; // Converted to boolean for QML
    int lowLimit = 65;
    int highLimit = 70;
    QString lowAlertFile = "/usr/share/sounds/jolla-ambient/stereo/general_warning.wav";
    QString highAlertFile = "/usr/share/sounds/jolla-ambient/stereo/positive_confirmation.wav";

    // To avoid repeating the same string over and over and over...
    const char* sLowAlert = "lowAlert";
    const char* sHighAlert = "highAlert";
    const char* sInterval = "interval";
    const char* sLimitEnabled = "limitEnabled";
    const char* sNotificationsEnabled = "notificationsEnabled";
    const char* sLowLimit = "lowLimit";
    const char* sHighLimit = "highLimit";
    const char* sLowAlertFile = "lowAlertFile";
    const char* sHighAlertFile = "highAlertFile";

    int bound(int value, int min, int max);
    void loadInteger(const char *key, int* value, int min, int max);

signals:
    int lowAlertChanged();
    int highAlertChanged();
    int intervalChanged();
    bool limitEnabledChanged();
    bool notificationsEnabledChanged();
    int lowLimitChanged();
    int highLimitChanged();
    QString lowAlertFileChanged();
    QString highAlertFileChanged();
};

#endif // SETTINGS_H
