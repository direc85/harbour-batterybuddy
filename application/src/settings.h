// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021 Peter G. <sailfish@nephros.org>

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
    Q_PROPERTY(int invertSign READ getInvertSign WRITE setInvertSign NOTIFY invertSignChanged)
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

    void setInvertSign(const int newSign);

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
    void invertSignChanged(int);
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
