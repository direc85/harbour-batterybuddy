/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2020 Matti Viljanen
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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include "logger.h"

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int highAlert READ getHighAlert WRITE setHighAlert NOTIFY highAlertChanged)
    Q_PROPERTY(int lowAlert READ getLowAlert WRITE setLowAlert NOTIFY lowAlertChanged)
    Q_PROPERTY(int highNotificationsInterval READ getHighNotificationsInterval WRITE setHighNotificationsInterval NOTIFY highNotificationsIntervalChanged)
    Q_PROPERTY(int lowNotificationsInterval READ getLowNotificationsInterval WRITE setLowNotificationsInterval NOTIFY lowNotificationsIntervalChanged)
    Q_PROPERTY(int highLimit READ getHighLimit WRITE setHighLimit NOTIFY highLimitChanged)
    Q_PROPERTY(int lowLimit READ getLowLimit WRITE setLowLimit NOTIFY lowLimitChanged)
    Q_PROPERTY(bool limitEnabled READ getLimitEnabled WRITE setLimitEnabled NOTIFY limitEnabledChanged)
    Q_PROPERTY(QString highAlertFile READ getHighAlertFile NOTIFY highAlertFileChanged)
    Q_PROPERTY(QString lowAlertFile READ getLowAlertFile NOTIFY lowAlertFileChanged)
    Q_PROPERTY(QString notificationTitle READ getNotificationTitle WRITE setNotificationTitle NOTIFY notificationTitleChanged)
    Q_PROPERTY(QString notificationLowText READ getNotificationLowText WRITE setNotificationLowText NOTIFY notificationLowTextChanged)
    Q_PROPERTY(QString notificationHighText READ getNotificationHighText WRITE setNotificationHighText NOTIFY notificationHighTextChanged)

public:
    Settings(Logger* newLogger, QObject* parent = nullptr);
    ~Settings();

    int  getLowAlert();
    int  getHighAlert();
    int  getHighNotificationsInterval();
    int  getLowNotificationsInterval();
    int  getLowLimit();
    int  getHighLimit();
    bool getLimitEnabled();
    QString getLowAlertFile();
    QString getHighAlertFile();
    QString getNotificationTitle();
    QString getNotificationLowText();
    QString getNotificationHighText();

    void setLowAlert(const int newLimit);
    void setHighAlert(const int newLimit);
    void setHighNotificationsInterval(const int newInterval);
    void setLowNotificationsInterval(const int newInterval);
    void setLowLimit(const int newLimit);
    void setHighLimit(const int newLimit);
    void setLimitEnabled(const bool newEnabled);
    void setNotificationTitle(const QString newText);
    void setNotificationLowText(const QString newText);
    void setNotificationHighText(const QString newText);

private:
    QSettings *mySettings = nullptr;
    const char* appName = APP_NAME;
    Logger* logger = nullptr;

    // Default values
    int lowAlert = 25;
    int highAlert = 75;
    int highNotificationsInterval = 60;
    int lowNotificationsInterval = 60;
    int limitEnabled = 1; // Converted to boolean for QML
    int lowLimit = 65;
    int highLimit = 70;
    QString lowAlertFile = "/usr/share/sounds/jolla-ambient/stereo/general_warning.wav";
    QString highAlertFile = "/usr/share/sounds/jolla-ambient/stereo/positive_confirmation.wav";
    QString notificationTitle;
    QString notificationLowText;
    QString notificationHighText;

    // To avoid repeating the same string over and over and over...
    const char* sLowAlert = "lowAlert";
    const char* sHighAlert = "highAlert";
    const char* sHighNotificationsInterval = "highNotificationsInterval";
    const char* sLowNotificationsInterval = "lowNotificationsInterval";
    const char* sLimitEnabled = "limitEnabled";
    const char* sLowLimit = "lowLimit";
    const char* sHighLimit = "highLimit";
    const char* sLowAlertFile = "lowAlertFile";
    const char* sHighAlertFile = "highAlertFile";
    const char* sNotificationTitle = "notificationTitle";
    const char* sNotificationLowText = "notificationLowText";
    const char* sNotificationHighText = "notificationHighText";

    void loadInteger(const char *key, int &value, const int min, const int max);
    void saveInteger(const char *key, const int &value);

signals:
    void lowAlertChanged(int);
    void highAlertChanged(int);
    void highNotificationsIntervalChanged(int);
    void lowNotificationsIntervalChanged(int);
    void limitEnabledChanged(bool);
    void lowLimitChanged(int);
    void highLimitChanged(int);
    void lowAlertFileChanged(QString);
    void highAlertFileChanged(QString);
    void notificationTitleChanged(QString);
    void notificationLowTextChanged(QString);
    void notificationHighTextChanged(QString);
};

#endif // SETTINGS_H
