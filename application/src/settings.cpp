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

Settings::Settings(QObject *parent) : QObject(parent)
{
    mySettings = new QSettings("harbour-batterybuddy", "harbour-batterybuddy");
    qDebug() << "Using" << mySettings->fileName();

    // Read in the values
    loadInteger(sLowAlert, &lowAlert, 10, 99);
    loadInteger(sHighAlert, &highAlert, 11, 100);
    loadInteger(sHighNotificationsInterval, &highNotificationsInterval, 50, 610);
    loadInteger(sLowNotificationsInterval, &lowNotificationsInterval, 50, 610);
    loadInteger(sLimitEnabled, &limitEnabled, 0, 1);
    loadInteger(sHighNotificationsEnabled, &highNotificationsEnabled, 0, 1);
    loadInteger(sLowNotificationsEnabled, &lowNotificationsEnabled, 0, 1);
    loadInteger(sLowLimit, &lowLimit, 20, 94);
    loadInteger(sHighLimit, &highLimit, 21, 95);

    notificationTitle = tr("Battery charge %1%");
    notificationLowText = tr("Please connect the charger.");
    notificationHighText = tr("Please disconnect the charger.");
}

Settings::~Settings()
{
    saveInteger(sLowAlert, &lowAlert);
    saveInteger(sHighAlert, &highAlert);
    saveInteger(sHighNotificationsInterval, &highNotificationsInterval);
    saveInteger(sLowNotificationsInterval, &lowNotificationsInterval);
    saveInteger(sLimitEnabled, &limitEnabled);
    saveInteger(sHighNotificationsEnabled, &highNotificationsEnabled);
    saveInteger(sLowNotificationsEnabled, &lowNotificationsEnabled);
    saveInteger(sLowLimit, &lowLimit);
    saveInteger(sHighLimit, &highLimit);
    mySettings->setValue(sNotificationTitle, notificationTitle);
    mySettings->setValue(sNotificationLowText, notificationLowText);
    mySettings->setValue(sNotificationHighText, notificationHighText);
    mySettings->sync();
    qInfo() << "Settings saved:" << (mySettings->status() == QSettings::NoError);
}

// Getters condensed.
int     Settings::getLowAlert()                  { return lowAlert; }
int     Settings::getHighAlert()                 { return highAlert; }
int     Settings::getHighNotificationsInterval() { return highNotificationsInterval; }
int     Settings::getLowNotificationsInterval()  { return lowNotificationsInterval; }
int     Settings::getLowLimit()                  { return lowLimit; }
int     Settings::getHighLimit()                 { return highLimit; }
bool    Settings::getLimitEnabled()              { return limitEnabled == 1; }
bool    Settings::getHighNotificationsEnabled()  { return highNotificationsEnabled == 1; }
bool    Settings::getLowNotificationsEnabled()   { return lowNotificationsEnabled == 1; }
QString Settings::getLowAlertFile()              { return lowAlertFile; }
QString Settings::getHighAlertFile()             { return highAlertFile; }
QString Settings::getNotificationTitle()         { return notificationTitle; }
QString Settings::getNotificationLowText()       { return notificationLowText; }
QString Settings::getNotificationHighText()      { return notificationHighText; }

void Settings::setLowAlert(int newLimit) {
    lowAlert = newLimit;
    saveInteger(sLowAlert, &lowAlert);
    // Lows and highs are always saved in pairs!
    //mySettings->sync();
    emit lowAlertChanged(lowAlert);
    qDebug() << sLowAlert << newLimit;
}

void Settings::setHighAlert(int newLimit) {
    highAlert = newLimit;
    saveInteger(sHighAlert, &highAlert);
    mySettings->sync();
    emit highAlertChanged(highAlert);
    qDebug() << sHighAlert << newLimit;
}

void Settings::setHighNotificationsInterval(int newInterval) {
    highNotificationsInterval = newInterval;
    saveInteger(sHighNotificationsInterval, &highNotificationsInterval);
    mySettings->sync();
    emit highNotificationsIntervalChanged(highNotificationsInterval);
    qDebug() << sHighNotificationsInterval << newInterval;
}

void Settings::setLowNotificationsInterval(int newInterval) {
    lowNotificationsInterval = newInterval;
    saveInteger(sLowNotificationsInterval, &lowNotificationsInterval);
    mySettings->sync();
    emit highNotificationsIntervalChanged(lowNotificationsInterval);
    qDebug() << sLowNotificationsInterval << newInterval;
}

void Settings::setLowLimit(int newLimit) {
    lowLimit = newLimit;
    saveInteger(sLowLimit, &lowLimit);
    // Lows and highs are always saved in pairs!
    //mySettings->sync();
    emit lowLimitChanged(lowLimit);
    qDebug() << sLowLimit << newLimit;
}

void Settings::setHighLimit(int newLimit) {
    highLimit = newLimit;
    saveInteger(sHighLimit, &highLimit);
    mySettings->sync();
    emit highLimitChanged(highLimit);
    qDebug() << sHighLimit << newLimit;
}

void Settings::setLimitEnabled(bool newEnabled) {
    limitEnabled = (newEnabled ? 1 : 0);
    saveInteger(sLimitEnabled, &limitEnabled);
    mySettings->sync();
    emit limitEnabledChanged(limitEnabled);
    qDebug() << sLimitEnabled << newEnabled;
}

void Settings::setHighNotificationsEnabled(bool newEnabled) {
    highNotificationsEnabled = (newEnabled ? 1 : 0);
    saveInteger(sHighNotificationsEnabled, &highNotificationsEnabled);
    mySettings->sync();
    emit highNotificationsEnabledChanged(highNotificationsEnabled);
    qDebug() << sHighNotificationsEnabled << newEnabled;
}

void Settings::setLowNotificationsEnabled(bool newEnabled) {
    lowNotificationsEnabled = (newEnabled ? 1 : 0);
    saveInteger(sLowNotificationsEnabled, &lowNotificationsEnabled);
    mySettings->sync();
    emit lowNotificationsEnabledChanged(lowNotificationsEnabled);
    qDebug() << sLowNotificationsEnabled << newEnabled;
}

void Settings::setNotificationTitle(QString newText) {
    notificationTitle = newText;
    mySettings->setValue(sNotificationTitle, notificationTitle);
    mySettings->sync();
    emit notificationTitleChanged(notificationTitle);
    qDebug() << sNotificationTitle << notificationTitle;
}

void Settings::setNotificationLowText(QString newText) {
    notificationLowText = newText;
    mySettings->setValue(sNotificationLowText, notificationLowText);
    mySettings->sync();
    emit notificationLowTextChanged(notificationLowText);
    qDebug() << sNotificationLowText << notificationLowText;
}

void Settings::setNotificationHighText(QString newText) {
    notificationHighText = newText;
    mySettings->setValue(sNotificationHighText, notificationHighText);
    mySettings->sync();
    emit notificationHighTextChanged(notificationHighText);
    qDebug() << sNotificationHighText << notificationHighText;
}

int Settings::bound(int value, int min, int max) {
    return (value <= min ? min : (value >= max ? max : value));
}

void Settings::loadInteger(const char* key, int *value, int min, int max) {
    *value = bound(mySettings->value(key, *value).toInt(), min, max);
    qInfo() << "Loaded" << key << *value;
}

void Settings::saveInteger(const char* key, int *value) {
    mySettings->setValue(key, QByteArray::number(*value));
    qInfo() << "Saved" << key << *value;
}
