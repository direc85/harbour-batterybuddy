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

    loadString(sLogFilename, logFilename);

    loadString(sNotificationTitle, notificationTitle);
    loadString(sNotificationLowText, notificationLowText);
    loadString(sNotificationHighText, notificationHighText);

    saveString(sNotificationTitle, tr("Battery charge %1%"), notificationTitle);
    saveString(sNotificationLowText, tr("Please connect the charger."), notificationLowText);
    saveString(sNotificationHighText, tr("Please disconnect the charger."), notificationHighText);
}

Settings::~Settings()
{
    mySettings->sync();
    logV(QString("Settings saved: %1").arg(mySettings->status() == QSettings::NoError));
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
    if(saveInteger(sLowAlert, newLimit, lowAlert)) {
        emit lowAlertChanged(lowAlert);
    }
}

void Settings::setHighAlert(const int newLimit) {
    if(saveInteger(sHighAlert, newLimit, highAlert)) {
        emit highAlertChanged(highAlert);
    }
}

void Settings::setHighNotificationsInterval(const int newInterval) {
    if(saveInteger(sHighNotificationsInterval, newInterval, highNotificationsInterval)) {
        emit highNotificationsIntervalChanged(highNotificationsInterval);
    }
}

void Settings::setLowNotificationsInterval(const int newInterval) {
    if(saveInteger(sLowNotificationsInterval, newInterval, lowNotificationsInterval)) {
        emit lowNotificationsIntervalChanged(lowNotificationsInterval);
    }
}

void Settings::setLowLimit(const int newLimit) {
    if(saveInteger(sLowLimit, newLimit, lowLimit)) {
        emit lowLimitChanged(lowLimit);
    }
}

void Settings::setHighLimit(const int newLimit) {
    if(saveInteger(sHighLimit, newLimit, highLimit))
        emit highLimitChanged(highLimit);
}

void Settings::setLimitEnabled(const bool newEnabled) {
    if(saveInteger(sLimitEnabled, (newEnabled ? 1 : 0), limitEnabled))
        emit limitEnabledChanged(limitEnabled);
}

void Settings::setNotificationTitle(const QString newText) {
    if(saveString(sNotificationTitle, newText, notificationTitle))
        emit notificationTitleChanged(notificationTitle);
}

void Settings::setNotificationLowText(const QString newText) {
    if(saveString(sNotificationLowText, newText, notificationLowText))
        emit notificationLowTextChanged(notificationLowText);
}

void Settings::setNotificationHighText(const QString newText) {
    if(saveString(sNotificationHighText, newText, notificationHighText))
        emit notificationHighTextChanged(notificationHighText);
}

void Settings::setLogLevel(const int newLogLevel) {
    if(saveInteger(sLogLevel, newLogLevel, logLevel))
        emit logLevelChanged(logLevel);
}

bool Settings::loadInteger(const char *key, int &currValue, const int min, const int max) {
    int newValue = mySettings->value(key, currValue).toInt();
    newValue = (newValue <= min ? min : (newValue >= max ? max : newValue));
    if(currValue == newValue) {
        logD(QString("Load: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    logV(QString("Load: %1 %2").arg(key).arg(currValue));
    return true;
}

bool Settings::loadString(const char *key, QString & currValue) {
    QString newValue = mySettings->value(key, currValue).toString();
    if(currValue == newValue) {
        logD(QString("Load: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    logV(QString("Load: %1 %2").arg(key).arg(currValue));
    return true;
}

bool Settings::saveInteger(const char* key, const int &newValue, int &currValue) {
    if(currValue == newValue) {
        logD(QString("Save: %1 %2 (unchanged)").arg(key).arg(currValue));
        return false;
    }
    currValue = newValue;
    mySettings->setValue(key, QByteArray::number(newValue));
    logV(QString("Save: %1 %2").arg(key).arg(newValue));
    return true;
}

bool Settings::saveString(const char* key, const QString &newValue, QString &currValue) {
    if(currValue == newValue) {
        return false;
    }
    currValue = newValue;
    mySettings->setValue(key, QString(newValue).replace("\"", "\\\"").toUtf8());
    logV(QString("Save: %1 %2").arg(key).arg(newValue));
    return true;
}
