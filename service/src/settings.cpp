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
    // Use the same file location as GUI for data exchange
    if(!mySettings) {
        mySettings = new QSettings("harbour-batterybuddy", "harbour-batterybuddy");
    }

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

    if(mySettings->contains("notificationsEnabled")) {
        mySettings->setValue("highNotificationsEnabled", mySettings->value("notificationsEnabled"));
        mySettings->remove("notificationsEnabled");
        qInfo() << "Migrated old notificationsEnabled value";
    }

    if(mySettings->contains("interval")) {
        mySettings->setValue(sHighNotificationsInterval, mySettings->value("interval"));
        mySettings->setValue(sLowNotificationsInterval, mySettings->value("interval"));
        mySettings->remove("interval");
        qInfo() << "Migrated old notification interval value";
    }

    if(mySettings->contains("highNotificationsEnabled")) {
        if(mySettings->value("highNotificationsEnabled").toInt() == 0)
            mySettings->setValue(sHighNotificationsInterval, 610);
        mySettings->remove("highNotificationsEnabled");
        qInfo() << "Migrated old highNotificationsEnabled value";
    }

    if(mySettings->contains("lowNotificationsEnabled")) {
        if(mySettings->value("lowNotificationsEnabled").toInt() == 0)
            mySettings->setValue(sLowNotificationsInterval, 610);
        mySettings->remove("lowNotificationsEnabled");
        qInfo() << "Migrated old lowNotificationsEnabled value";
    }

    // Do this here, because...
    watcher = new QFileSystemWatcher(QStringList(mySettings->fileName()));
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateConfig(QString)));

    // ...calling this deletes mySettings!
    updateConfig(mySettings->fileName());

    qInfo() << "Loaded" << sLimitEnabled << limitEnabled;

    // Battery Buddy GUI application changes the settings file,
    // so we must monitor it and update when it changes.
}

Settings::~Settings()
{
    delete mySettings;
}

int Settings::bound(int value, int min, int max) {
    return (value <= min ? min : (value >= max ? max : value));
}

bool Settings::loadInteger(const char* key, int *value, int min, int max) {
    oldValue = *value;
    *value = bound(mySettings->value(key, *value).toInt(), min, max);
    if(oldValue != *value) {
        qInfo() << "Loaded" << key << *value;
    }
    return oldValue != *value;
}

void Settings::updateConfig(QString path) {

    // Use the same file location as GUI for data exchange
    if(!mySettings) {
        mySettings = new QSettings("harbour-batterybuddy", "harbour-batterybuddy");
    }

    qDebug() << "Reading values...";
    // Read in the values
    bool restartTimers = false;

    loadInteger(sLowAlert, &lowAlert, 10, 99);
    loadInteger(sHighAlert, &highAlert, 11, 100);
    restartTimers |= loadInteger(sHighNotificationsInterval, &highNotificationsInterval, 50, 610);
    restartTimers |= loadInteger(sLowNotificationsInterval, &lowNotificationsInterval, 50, 610);
    loadInteger(sLimitEnabled, &limitEnabled, 0, 1);
    loadInteger(sLowLimit, &lowLimit, 20, 94);
    loadInteger(sHighLimit, &highLimit, 21, 95);

    // These are translated in the GUI application
    // and delivered here via the config file
    notificationTitle = mySettings->value(sNotificationTitle, "Battery charge %1%").toString();
    notificationLowText = mySettings->value(sNotificationLowText, "Please connect the charger.").toString();
    notificationHighText = mySettings->value(sNotificationHighText, "Please disconnect the charger.").toString();

    qDebug() << "Values read.";

    delete mySettings;
    mySettings = nullptr;

    // Let the file system settle...
    QThread::msleep(50);

    if(watcher->files().contains(path)) {
        qDebug() << "File OK";
    }
    else {
        qDebug() << "File replaced, re-adding.";
        watcher->addPath(path);
    }

    if(restartTimers) {
        emit resetTimers();
    }
}

// Getters condensed
int     Settings::getLowAlert()                  { return lowAlert; }
int     Settings::getHighAlert()                 { return highAlert; }
int     Settings::getHighNotificationsInterval() { return highNotificationsInterval; }
int     Settings::getLowNotificationsInterval()  { return lowNotificationsInterval; }
int     Settings::getLowLimit()                  { return lowLimit; }
int     Settings::getHighLimit()                 { return highLimit; }
bool    Settings::getLimitEnabled()              { return limitEnabled == 1; }
QString Settings::getLowAlertFile()              { return lowAlertFile; }
QString Settings::getHighAlertFile()             { return highAlertFile; }
QString Settings::getNotificationTitle()         { return notificationTitle; }
QString Settings::getNotificationLowText()       { return notificationLowText; }
QString Settings::getNotificationHighText()      { return notificationHighText; }
