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
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    // Migrate old settings
    if(mySettings.contains("lowerLimit")) {
        mySettings.setValue(sLowAlert, mySettings.value("lowerLimit"));
        mySettings.remove("lowerLimit");
        qInfo() << "Migrated old lowerLimit value";
    }

    if(mySettings.contains("upperLimit")) {
        mySettings.setValue(sHighAlert, mySettings.value("upperLimit"));
        mySettings.remove("upperLimit");
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
    qInfo() << "Loaded" << sLimitEnabled << limitEnabled;
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
    mySettings.sync();
    qInfo() << "Settings saved:" << mySettings.status() == QSettings::NoError;
}

int  Settings::getLowAlert() {
    return lowAlert;
}

int  Settings::getHighAlert() {
    return highAlert;
}

int  Settings::getInterval() {
    return interval;
}

int  Settings::getLowLimit() {
    return lowLimit;
}

int  Settings::getHighLimit() {
    return highLimit;
}

bool Settings::getLimitEnabled() {
    return limitEnabled == 1;
}

bool Settings::getNotificationsEnabled() {
    return notificationsEnabled == 1;
}

QString Settings::getLowAlertFile() {
    return lowAlertFile;
}

QString Settings::getHighAlertFile() {
    return highAlertFile;
}

void Settings::setLowAlert(int newLimit) {
    lowAlert = newLimit;
    emit lowAlertChanged();
    qDebug() << "Change" << sLowAlert << newLimit;
}

void Settings::setHighAlert(int newLimit) {
    highAlert = newLimit;
    emit highAlertChanged();
    qDebug() << "Change" << sHighAlert << newLimit;
}

void Settings::setInterval(int newInterval) {
    interval = newInterval;
    emit intervalChanged();
    qDebug() << "Change" << sInterval << newInterval;
}

void Settings::setLowLimit(int newLimit) {
    lowLimit = newLimit;
    emit lowLimitChanged();
    qDebug() << "Change" << sLowLimit << newLimit;
}

void Settings::setHighLimit(int newLimit) {
    highLimit = newLimit;
    emit highLimitChanged();
    qDebug() << "Change" << sHighLimit << newLimit;
}

void Settings::setLimitEnabled(bool newEnabled) {
    limitEnabled = (newEnabled ? 1 : 0);
    emit limitEnabledChanged();
    qDebug() << "Change" << sLimitEnabled << newEnabled;
}

void Settings::setNotificationsEnabled(bool newEnabled) {
    notificationsEnabled = (newEnabled ? 1 : 0);
    emit notificationsEnabledChanged();
    qDebug() << "Change" << sNotificationsEnabled << newEnabled;
}


int Settings::bound(int value, int min, int max) {
    return (value <= min ? min : (value >= max ? max : value));
}

void Settings::loadInteger(const char* key, int *value, int min, int max) {
    *value = bound(mySettings.value(key, *value).toInt(), min, max);
    qInfo() << "Loaded" << key << *value;
}

void Settings::saveInteger(const char* key, int *value) {
    mySettings.setValue(key, QByteArray::number(*value));
    qInfo() << "Saved" << key << *value;
}
