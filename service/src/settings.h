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
#include <QFileSystemWatcher>
#include <QThread>
#include "logger.h"

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings(Logger* newLogger, QObject* parent = nullptr);
    ~Settings();

    int  getLowAlert();
    int  getHighAlert();
    int  getHighNotificationsInterval();
    int  getLowNotificationsInterval();
    int  getHealthAlert();
    int  getHealthNotificationsInterval();
    int  getLowLimit();
    int  getHighLimit();
    int  getLogLevel();
    bool getLimitEnabled();
    bool getHighNotificationsEnabled();
    bool getLowNotificationsEnabled();
    bool getHealthNotificationsEnabled();
    QString getLowAlertFile();
    QString getHighAlertFile();
    QString getHealthAlertFile();
    QString getNotificationTitle();
    QString getNotificationLowText();
    QString getNotificationHighText();
    QString getNotificationHealthTitle();
    QString getNotificationHealthWarnText();
    QString getNotificationHealthCritText();

private:
    Logger* logger;
    QSettings* mySettings = nullptr;
    QFileSystemWatcher *watcher = nullptr;

    // Set this manually, because we want to use
    // the same config file as the GUI application
    const char* appName = "harbour-batterybuddy";

    int oldValue;
    int logLevel = 1;

    // Default values
    int lowAlert = 25;
    int highAlert = 75;
    int healthAlert = 1; // 0=off, 1=warn, 2=crit
    int highNotificationsInterval = 60;
    int lowNotificationsInterval = 60;
    int healthNotificationsInterval = 60;

    // Converted to boolean for QML
    int limitEnabled = 1;
    int daemonEnabled = 1;

    int lowLimit = 65;
    int highLimit = 70;
    QString lowAlertFile = "/usr/share/sounds/jolla-ambient/stereo/battery_low.wav";
    QString highAlertFile = "/usr/share/sounds/jolla-ambient/stereo/positive_confirmation.wav";
    QString healthAlertFile = "/usr/share/sounds/jolla-ambient/stereo/general_warning.wav";
    QString notificationTitle;
    QString notificationLowText;
    QString notificationHighText;
    QString notificationHealthTitle;
    QString notificationHealthWarnText;
    QString notificationHealthCritText;

    // To avoid repeating the same string over and over and over...
    const char* sLowAlert = "lowAlert";
    const char* sHighAlert = "highAlert";
    const char* sHealthAlert = "healthAlert";
    const char* sHighNotificationsInterval = "highNotificationsInterval";
    const char* sLowNotificationsInterval = "lowNotificationsInterval";
    const char* sHealthNotificationsInterval = "healthNotificationsInterval";
    const char* sLimitEnabled = "limitEnabled";
    const char* sLowLimit = "lowLimit";
    const char* sHighLimit = "highLimit";
    const char* sLowAlertFile = "lowAlertFile";
    const char* sHighAlertFile = "highAlertFile";
    const char* sHealthAlertFile = "healthAlertFile";
    const char* sNotificationTitle = "notificationTitle";
    const char* sNotificationLowText = "notificationLowText";
    const char* sNotificationHighText = "notificationHighText";
    const char* sNotificationHealthTitle = "notificationHealthTitle";
    const char* sNotificationHealthWarnText = "notificationHealthWarnText";
    const char* sNotificationHealthCritText = "notificationHealthCritText";
    const char* sLogFilename = "logFilename";
    const char* sLogLevel = "logLevel";

    bool loadInteger(const char *key, int &currValue, const int min, const int max);

private slots:
    void updateConfig(const QString path);

signals:
    void resetTimers();
};

#endif // SETTINGS_H
