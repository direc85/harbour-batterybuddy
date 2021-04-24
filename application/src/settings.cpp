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
#include "settings.h"

Settings::Settings(Logger *newLogger, QObject *parent) : QObject(parent)
{
    // Use the same file location as GUI for data exchange
    if(!mySettings) {
        mySettings = new QSettings(appName, appName, this);
    }

    logger = newLogger;

    logV("Using " + mySettings->fileName());

    // Read in the values
    loadInteger(sLowAlert, lowAlert, 5, 99);
    loadInteger(sHighAlert, highAlert, 6, 100);
    loadInteger(sHighNotificationsInterval, highNotificationsInterval, 50, 610);
    loadInteger(sLowNotificationsInterval, lowNotificationsInterval, 50, 610);
    loadInteger(sLimitEnabled, limitEnabled, 0, 1);
    loadInteger(sLowLimit, lowLimit, 5, 99);
    loadInteger(sHighLimit, highLimit, 6, 100);
    loadInteger(sLogLevel, logLevel, 0, 2);
    logFilename = mySettings->value(sLogFilename).toString();

    notificationTitle = tr("Battery charge %1%");
    notificationLowText = tr("Please connect the charger.");
    notificationHighText = tr("Please disconnect the charger.");
}

Settings::~Settings()
{
    saveInteger(sLowAlert, lowAlert);
    saveInteger(sHighAlert, highAlert);
    saveInteger(sHighNotificationsInterval, highNotificationsInterval);
    saveInteger(sLowNotificationsInterval, lowNotificationsInterval);
    saveInteger(sLimitEnabled, limitEnabled);
    saveInteger(sLowLimit, lowLimit);
    saveInteger(sHighLimit, highLimit);
    mySettings->setValue(sNotificationTitle, notificationTitle);
    mySettings->setValue(sNotificationLowText, notificationLowText);
    mySettings->setValue(sNotificationHighText, notificationHighText);
    mySettings->sync();
    logV(QString("Settings saved: %1").arg((mySettings->status() == QSettings::NoError) ? "true" : "false"));
}

// Getters condensed.
int     Settings::getLowAlert()                  { return lowAlert; }
int     Settings::getHighAlert()                 { return highAlert; }
int     Settings::getHighNotificationsInterval() { return highNotificationsInterval; }
int     Settings::getLowNotificationsInterval()  { return lowNotificationsInterval; }
int     Settings::getLowLimit()                  { return lowLimit; }
int     Settings::getHighLimit()                 { return highLimit; }
bool    Settings::getLimitEnabled()              { return limitEnabled == 1; }
QString Settings::getLowAlertFile()              { return lowAlertFile; }
QString Settings::getHighAlertFile()             { return highAlertFile; }
QString Settings::getLogFilename()               { return logFilename; }
QString Settings::getNotificationTitle()         { return notificationTitle; }
QString Settings::getNotificationLowText()       { return notificationLowText; }
QString Settings::getNotificationHighText()      { return notificationHighText; }
int     Settings::getLogLevel()                  { return logLevel; }

void Settings::setLowAlert(const int newLimit) {
    lowAlert = newLimit;
    saveInteger(sLowAlert, lowAlert);
    // Lows and highs are always saved in pairs!
    //mySettings->sync();
    emit lowAlertChanged(lowAlert);
    logD(QString("%1 %2").arg(sLowAlert).arg(newLimit));
}

void Settings::setHighAlert(const int newLimit) {
    highAlert = newLimit;
    saveInteger(sHighAlert, highAlert);
    mySettings->sync();
    emit highAlertChanged(highAlert);
}

void Settings::setHighNotificationsInterval(const int newInterval) {
    highNotificationsInterval = newInterval;
    saveInteger(sHighNotificationsInterval, highNotificationsInterval);
    mySettings->sync();
    emit highNotificationsIntervalChanged(highNotificationsInterval);
}

void Settings::setLowNotificationsInterval(const int newInterval) {
    lowNotificationsInterval = newInterval;
    saveInteger(sLowNotificationsInterval, lowNotificationsInterval);
    mySettings->sync();
    emit highNotificationsIntervalChanged(lowNotificationsInterval);
}

void Settings::setLowLimit(const int newLimit) {
    lowLimit = newLimit;
    saveInteger(sLowLimit, lowLimit);
    // Lows and highs are always saved in pairs!
    //mySettings->sync();
    emit lowLimitChanged(lowLimit);
}

void Settings::setHighLimit(const int newLimit) {
    highLimit = newLimit;
    saveInteger(sHighLimit, highLimit);
    mySettings->sync();
    emit highLimitChanged(highLimit);
}

void Settings::setLimitEnabled(const bool newEnabled) {
    limitEnabled = (newEnabled ? 1 : 0);
    saveInteger(sLimitEnabled, limitEnabled);
    mySettings->sync();
    emit limitEnabledChanged(limitEnabled);
}

void Settings::setNotificationTitle(const QString newText) {
    notificationTitle = QString(newText).replace("\"", "\\\"");
    mySettings->setValue(sNotificationTitle, notificationTitle);
    mySettings->sync();
    emit notificationTitleChanged(notificationTitle);
}

void Settings::setNotificationLowText(const QString newText) {
    notificationLowText = QString(newText).replace("\"", "\\\"");
    mySettings->setValue(sNotificationLowText, notificationLowText);
    mySettings->sync();
    emit notificationLowTextChanged(notificationLowText);
}

void Settings::setNotificationHighText(const QString newText) {
    notificationHighText = QString(newText).replace("\"", "\\\"");
    mySettings->setValue(sNotificationHighText, notificationHighText);
    mySettings->sync();
    emit notificationHighTextChanged(notificationHighText);
}

void Settings::setLogLevel(const int newLogLevel) {
    logLevel = newLogLevel;
    saveInteger(sLogLevel, logLevel);
    mySettings->sync();
    emit logLevelChanged(logLevel);
}

void Settings::loadInteger(const char *key, int &value, const int min, const int max) {
    int newValue = mySettings->value(key, value).toInt();
    value = (newValue <= min ? min : (newValue >= max ? max : newValue));
    logV(QString("Load: %1 %2").arg(key).arg(value));
}

void Settings::saveInteger(const char* key, const int &value) {
    mySettings->setValue(key, QByteArray::number(value));
    logV(QString("Save: %1 %2").arg(key).arg(value));
}
