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

    // Migrate old settings
    if(mySettings->contains("lowerLimit")) {
        mySettings->setValue(sLowAlert, mySettings->value("lowerLimit"));
        mySettings->remove("lowerLimit");
        qInfo() << "Migrated old lowerLimit value";
    }

    if(mySettings->contains("upperLimit")) {
        mySettings->setValue(sHighAlert, mySettings->value("upperLimit"));
        mySettings->remove("upperLimit");
        qInfo() << "Migrated old upperLimit value";
    }

    // Read in the values
    loadInteger(sLowAlert, &lowAlert, 10, 99);
    loadInteger(sHighAlert, &highAlert, 11, 100);
    loadInteger(sInterval, &interval, 60, 600);
    loadInteger(sLimitEnabled, &limitEnabled, 0, 1);
    loadInteger(sNotificationsEnabled, &notificationsEnabled, 0, 1);
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
    saveInteger(sInterval, &interval);
    saveInteger(sLimitEnabled, &limitEnabled);
    saveInteger(sNotificationsEnabled, &notificationsEnabled);
    saveInteger(sLowLimit, &lowLimit);
    saveInteger(sHighLimit, &highLimit);
    mySettings->setValue(sNotificationTitle, notificationTitle);
    mySettings->setValue(sNotificationLowText, notificationLowText);
    mySettings->setValue(sNotificationHighText, notificationHighText);
    mySettings->sync();
    qInfo() << "Settings saved:" << (mySettings->status() == QSettings::NoError);
}

// Getters condensed.
int     Settings::getLowAlert()             { return lowAlert; }
int     Settings::getHighAlert()            { return highAlert; }
int     Settings::getInterval()             { return interval; }
int     Settings::getLowLimit()             { return lowLimit; }
int     Settings::getHighLimit()            { return highLimit; }
bool    Settings::getLimitEnabled()         { return limitEnabled == 1; }
bool    Settings::getNotificationsEnabled() { return notificationsEnabled == 1; }
QString Settings::getLowAlertFile()         { return lowAlertFile; }
QString Settings::getHighAlertFile()        { return highAlertFile; }
QString Settings::getNotificationTitle()    { return notificationTitle; }
QString Settings::getNotificationLowText()  { return notificationLowText; }
QString Settings::getNotificationHighText() { return notificationHighText; }

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

void Settings::setInterval(int newInterval) {
    interval = newInterval;
    saveInteger(sInterval, &interval);
    mySettings->sync();
    emit intervalChanged(interval);
    qDebug() << sInterval << newInterval;
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

void Settings::setNotificationsEnabled(bool newEnabled) {
    notificationsEnabled = (newEnabled ? 1 : 0);
    saveInteger(sNotificationsEnabled, &notificationsEnabled);
    mySettings->sync();
    emit notificationsEnabledChanged(notificationsEnabled);
    qDebug() << sNotificationsEnabled << newEnabled;
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
