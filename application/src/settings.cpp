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
#include "settings.h"

Settings::Settings(Logger *newLogger, QObject *parent) : SettingsBase(newLogger, parent)
{
    // Use the same file location as GUI for data exchange
    if(!settings) {
        settings = new QSettings(appName, appName, this);
    }

    logM("Using " + settings->fileName());

    // Read in the values
    loadInteger(sLowAlert, lowAlert, 5, 99);
    loadInteger(sHighAlert, highAlert, 6, 100);
    loadInteger(sHealthAlert, healthAlert, 0, 2);
    loadInteger(sHighNotificationsInterval, highNotificationsInterval, 0, 13);
    loadInteger(sLowNotificationsInterval, lowNotificationsInterval, 0, 13);
    loadInteger(sHealthNotificationsInterval, healthNotificationsInterval, 0, 13);
    loadInteger(sLimitEnabled, limitEnabled, 0, 1);
    loadInteger(sLowLimit, lowLimit, 5, 99);
    loadInteger(sHighLimit, highLimit, 6, 100);
    loadInteger(sMaxSupportedChargeCurrent, maxSupportedChargeCurrent, 0, 5000000);
    loadInteger(sMaxChargeCurrent, maxChargeCurrent, 0, maxSupportedChargeCurrent);
    loadInteger(sLogLevel, logLevel, 0, 2);

    loadString(sLogFilename, logFilename);

    loadString(sNotificationTitle, notificationTitle);
    loadString(sNotificationLowText, notificationLowText);
    loadString(sNotificationHighText, notificationHighText);
    loadString(sNotificationHealthTitle, notificationHealthTitle);
    loadString(sNotificationHealthWarnText, notificationHealthWarnText);
    loadString(sNotificationHealthCritText, notificationHealthCritText);

    saveString(sNotificationTitle, tr("Battery charge %1%"), notificationTitle);
    saveString(sNotificationLowText, tr("Please connect the charger."), notificationLowText);
    saveString(sNotificationHighText, tr("Please disconnect the charger."), notificationHighText);
    saveString(sNotificationHealthTitle, tr("Battery health %1"), notificationHealthTitle);
    saveString(sNotificationHealthWarnText, tr("Battery health is not good"), notificationHealthWarnText);
    saveString(sNotificationHealthCritText, tr("Battery health is critical"), notificationHealthCritText);

    connect(this, &SettingsBase::logLevelChanged, this, &Settings::logLevelChanged);
    connect(this, &SettingsBase::lowAlertChanged, this, &Settings::lowAlertChanged);
    connect(this, &SettingsBase::highAlertChanged, this, &Settings::highAlertChanged);
    connect(this, &SettingsBase::healthAlertChanged, this, &Settings::healthAlertChanged);
    connect(this, &SettingsBase::highNotificationsIntervalChanged, this, &Settings::highNotificationsIntervalChanged);
    connect(this, &SettingsBase::lowNotificationsIntervalChanged, this, &Settings::lowNotificationsIntervalChanged);
    connect(this, &SettingsBase::healthNotificationsIntervalChanged, this, &Settings::healthNotificationsIntervalChanged);
    connect(this, &SettingsBase::limitEnabledChanged, this, &Settings::limitEnabledChanged);
    connect(this, &SettingsBase::lowLimitChanged, this, &Settings::lowLimitChanged);
    connect(this, &SettingsBase::highLimitChanged, this, &Settings::highLimitChanged);
    connect(this, &SettingsBase::maxChargeCurrentChanged, this, &Settings::maxChargeCurrentChanged);
    connect(this, &SettingsBase::maxSupportedChargeCurrentChanged, this, &Settings::maxSupportedChargeCurrentChanged);
    connect(this, &SettingsBase::lowAlertFileChanged, this, &Settings::lowAlertFileChanged);
    connect(this, &SettingsBase::highAlertFileChanged, this, &Settings::highAlertFileChanged);
    connect(this, &SettingsBase::healthAlertFileChanged, this, &Settings::healthAlertFileChanged);
    connect(this, &SettingsBase::notificationTitleChanged, this, &Settings::notificationTitleChanged);
    connect(this, &SettingsBase::notificationLowTextChanged, this, &Settings::notificationLowTextChanged);
    connect(this, &SettingsBase::notificationHighTextChanged, this, &Settings::notificationHighTextChanged);
    connect(this, &SettingsBase::notificationHealthTitleChanged, this, &Settings::notificationHealthTitleChanged);
    connect(this, &SettingsBase::notificationHealthWarnTextChanged, this, &Settings::notificationHealthWarnTextChanged);
    connect(this, &SettingsBase::notificationHealthCritTextChanged, this, &Settings::notificationHealthCritTextChanged);
    connect(this, &SettingsBase::logFilenameChanged, this, &Settings::logFilenameChanged);
}

Settings::~Settings()
{
    settings->sync();
    logM(QString("Settings saved: %1").arg(settings->status() == QSettings::NoError));
}
