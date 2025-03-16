/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2022 Matti Viljanen
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
#include "logger.h"
#include "settingsbase.h"

class Settings : public SettingsBase
{
    Q_OBJECT
    Q_PROPERTY(int highAlert READ getHighAlert WRITE setHighAlert NOTIFY highAlertChanged)
    Q_PROPERTY(int lowAlert READ getLowAlert WRITE setLowAlert NOTIFY lowAlertChanged)
    Q_PROPERTY(int healthAlert READ getHealthAlert WRITE setHealthAlert NOTIFY healthAlertChanged)
    Q_PROPERTY(int highNotificationsInterval READ getHighNotificationsInterval WRITE setHighNotificationsInterval NOTIFY highNotificationsIntervalChanged)
    Q_PROPERTY(int lowNotificationsInterval READ getLowNotificationsInterval WRITE setLowNotificationsInterval NOTIFY lowNotificationsIntervalChanged)
    Q_PROPERTY(int healthNotificationsInterval READ getHealthNotificationsInterval WRITE setHealthNotificationsInterval NOTIFY healthNotificationsIntervalChanged)
    Q_PROPERTY(int highLimit READ getHighLimit WRITE setHighLimit NOTIFY highLimitChanged)
    Q_PROPERTY(int lowLimit READ getLowLimit WRITE setLowLimit NOTIFY lowLimitChanged)
    Q_PROPERTY(int maxChargeCurrent READ getMaxChargeCurrent WRITE setMaxChargeCurrent NOTIFY maxChargeCurrentChanged)
    Q_PROPERTY(int maxSupportedChargeCurrent READ getMaxSupportedChargeCurrent NOTIFY maxSupportedChargeCurrentChanged)
    Q_PROPERTY(bool limitEnabled READ getLimitEnabled WRITE setLimitEnabled NOTIFY limitEnabledChanged)
    Q_PROPERTY(QString highAlertFile READ getHighAlertFile NOTIFY highAlertFileChanged)
    Q_PROPERTY(QString lowAlertFile READ getLowAlertFile NOTIFY lowAlertFileChanged)
    Q_PROPERTY(QString healthAlertFile READ getHealthAlertFile NOTIFY healthAlertFileChanged)
    Q_PROPERTY(QString notificationTitle READ getNotificationTitle WRITE setNotificationTitle NOTIFY notificationTitleChanged)
    Q_PROPERTY(QString notificationLowText READ getNotificationLowText WRITE setNotificationLowText NOTIFY notificationLowTextChanged)
    Q_PROPERTY(QString notificationHighText READ getNotificationHighText WRITE setNotificationHighText NOTIFY notificationHighTextChanged)
    Q_PROPERTY(QString notificationHealthTitle READ getNotificationHealthTitle WRITE setNotificationHealthTitle NOTIFY notificationHealthTitleChanged)
    Q_PROPERTY(QString notificationHealthWarnText READ getNotificationHealthWarnText WRITE setNotificationHealthWarnText NOTIFY notificationHealthWarnTextChanged)
    Q_PROPERTY(QString notificationHealthCritText READ getNotificationHealthCritText WRITE setNotificationHealthCritText NOTIFY notificationHealthCritTextChanged)
    Q_PROPERTY(QString logFilename READ getLogFilename NOTIFY logFilenameChanged)
    Q_PROPERTY(int logLevel READ getLogLevel WRITE setLogLevel NOTIFY logLevelChanged)

public:
    Settings(Logger* newLogger, QObject* parent = nullptr);
    ~Settings();

signals:
    void lowAlertChanged(int);
    void highAlertChanged(int);
    void healthAlertChanged(int);
    void highNotificationsIntervalChanged(int);
    void lowNotificationsIntervalChanged(int);
    void healthNotificationsIntervalChanged(int);
    void limitEnabledChanged(bool);
    void lowLimitChanged(int);
    void highLimitChanged(int);
    void maxChargeCurrentChanged(int);
    void maxSupportedChargeCurrentChanged(int);
    void lowAlertFileChanged(QString);
    void highAlertFileChanged(QString);
    void healthAlertFileChanged(QString);
    void notificationTitleChanged(QString);
    void notificationLowTextChanged(QString);
    void notificationHighTextChanged(QString);
    void notificationHealthTitleChanged(QString);
    void notificationHealthWarnTextChanged(QString);
    void notificationHealthCritTextChanged(QString);
    void logFilenameChanged(QString);
    void logLevelChanged(int);
};

#endif // SETTINGS_H
