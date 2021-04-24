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

Settings::Settings(Logger* newLogger, QObject *parent) : QObject(parent)
{
    logger = newLogger;
    // Use the same file location as GUI for data exchange
    if(!mySettings) {
        mySettings = new QSettings(appName, appName, this);
    }

    logV("Using " + mySettings->fileName());

    QString logFilename = logger->getLogFilename();
    if(mySettings->value(sLogFilename,QString()).toString() != logFilename) {
        mySettings->setValue(sLogFilename, logFilename);
    }

    QString migrate = "Migrated value %1";
    QString key = "";

    // Migrate old settings
    key = "lowerLimit";
    if(mySettings->contains(key)) {
        mySettings->setValue(sLowAlert, mySettings->value(key));
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    key = "upperLimit";
    if(mySettings->contains(key)) {
        mySettings->setValue(sHighAlert, mySettings->value(key));
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    key = "notificationsEnabled";
    if(mySettings->contains(key)) {
        if(mySettings->value(key).toInt() == 0) {
            mySettings->setValue(sHighNotificationsInterval, 610);
            mySettings->setValue(sLowNotificationsInterval, 610);
        }
        else {
            mySettings->setValue(sHighNotificationsInterval, highNotificationsInterval);
            mySettings->setValue(sLowNotificationsInterval, lowNotificationsInterval);
        }
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    key = "interval";
    if(mySettings->contains(key)) {
        mySettings->setValue(sHighNotificationsInterval, mySettings->value(key));
        mySettings->setValue(sLowNotificationsInterval, mySettings->value(key));
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    key = "highNotificationsEnabled";
    if(mySettings->contains(key)) {
        if(mySettings->value(key).toInt() == 0)
            mySettings->setValue(sHighNotificationsInterval, 610);
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    key = "lowNotificationsEnabled";
    if(mySettings->contains(key)) {
        if(mySettings->value(key).toInt() == 0)
            mySettings->setValue(sLowNotificationsInterval, 610);
        mySettings->remove(key);
        logV(migrate.arg(key));
    }

    // Do this here, because...
    watcher = new QFileSystemWatcher(QStringList(mySettings->fileName()), this);
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateConfig(QString)));

    // ...calling this deletes mySettings!
    updateConfig(mySettings->fileName());

    // Battery Buddy GUI application changes the settings file,
    // so we must monitor it and update when it changes.
}

Settings::~Settings() { }

bool Settings::loadInteger(const char *key, int &value, const int min, const int max) {
    oldValue = value;
    value = mySettings->value(key, value).toInt();
    value = (value <= min ? min : (value >= max ? max : value));
    if(oldValue != value) {
        logV(QString("Load: %1 %2").arg(key).arg(value));
    }
    return oldValue != value;
}

void Settings::updateConfig(const QString path) {

    // Use the same file location as GUI for data exchange
    if(!mySettings) {
        mySettings = new QSettings(appName, appName, this);
    }

    logD("Updating configuration...");

    // Read in the values
    bool restartTimers = false;

    loadInteger(sLowAlert, lowAlert, 5, 99);
    loadInteger(sHighAlert, highAlert, 6, 100);
    restartTimers |= loadInteger(sHighNotificationsInterval, highNotificationsInterval, 50, 610);
    restartTimers |= loadInteger(sLowNotificationsInterval, lowNotificationsInterval, 50, 610);
    loadInteger(sLimitEnabled, limitEnabled, 0, 1);
    loadInteger(sLowLimit, lowLimit, 5, 99);
    loadInteger(sHighLimit, highLimit, 6, 100);

    // Update log level
    int oldLogLevel = logLevel;
    loadInteger(sLogLevel, logLevel, 0, 2);
        if(oldLogLevel != logLevel) {
        logger->debug = (logLevel == 2);
        logger->verbose = (logLevel > 1);
        logE(QString("Log level set to %1").arg((logLevel == 0 ? "low" : (logLevel == 1 ? "medium" : "high"))));
    }


    // These are translated in the GUI application
    // and delivered here via the config file
    notificationTitle = mySettings->value(sNotificationTitle, "Battery charge %1%").toString();
    notificationLowText = mySettings->value(sNotificationLowText, "Please connect the charger.").toString();
    notificationHighText = mySettings->value(sNotificationHighText, "Please disconnect the charger.").toString();

    delete mySettings;
    mySettings = nullptr;

    // Let the file system settle...
    QThread::msleep(100);

    if(watcher->files().contains(path)) {
        logD("Config file already on watchlist");
    }
    else {
        watcher->addPath(path);
        logD("Config file added to watchlist.");
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
int     Settings::getLogLevel()                  { return logLevel; }
