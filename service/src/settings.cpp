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
#include "settings.h"

Settings::Settings(Logger* newLogger, QObject *parent)
    : SettingsBase(newLogger, parent)
{
    // Use the same file location as GUI for data exchange
    settings = new QSettings(appName, appName, this);

    logL("Using " + settings->fileName());

    QString logFilename = logger->getLogFilename();
    if(settings->value(sLogFilename,QString()).toString() != logFilename) {
        settings->setValue(sLogFilename, logFilename);
    }

    loadString(sNotificationTitle, notificationTitle);
    loadString(sNotificationLowText, notificationLowText);
    loadString(sNotificationHighText, notificationHighText);
    loadString(sNotificationHealthTitle, notificationHealthTitle);
    loadString(sNotificationHealthWarnText, notificationHealthWarnText);
    loadString(sNotificationHealthCritText, notificationHealthCritText);

    QString migrate = "Migrated value %1";
    QString key = "";

    // Migrate old settings
    key = "lowerLimit";
    if(settings->contains(key)) {
        settings->setValue(sLowAlert, settings->value(key));
        settings->remove(key);
        logM(migrate.arg(key));
    }

    key = "upperLimit";
    if(settings->contains(key)) {
        settings->setValue(sHighAlert, settings->value(key));
        settings->remove(key);
        logM(migrate.arg(key));
    }

    key = "notificationsEnabled";
    if(settings->contains(key)) {
        if(settings->value(key).toInt() == 0) {
            settings->setValue(sHighNotificationsInterval, 2);
            settings->setValue(sLowNotificationsInterval, 2);
        }
        else {
            settings->setValue(sHighNotificationsInterval, highNotificationsInterval);
            settings->setValue(sLowNotificationsInterval, lowNotificationsInterval);
        }
        settings->remove(key);
        logM(migrate.arg(key));
    }

    key = "interval";
    if(settings->contains(key)) {
        settings->setValue(sHighNotificationsInterval, settings->value(key));
        settings->setValue(sLowNotificationsInterval, settings->value(key));
        settings->remove(key);
        logM(migrate.arg(key));
    }

    key = "highNotificationsEnabled";
    if(settings->contains(key)) {
        if(settings->value(key).toInt() == 0)
            settings->setValue(sHighNotificationsInterval, 2);
        settings->remove(key);
        logM(migrate.arg(key));
    }

    key = "lowNotificationsEnabled";
    if(settings->contains(key)) {
        if(settings->value(key).toInt() == 0)
            settings->setValue(sLowNotificationsInterval, 2);
        settings->remove(key);
        logM(migrate.arg(key));
    }

    watcher = new QFileSystemWatcher(QStringList(settings->fileName()), this);
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateConfig(QString)));

    // To trigger setting the initial config value
    maxChargeCurrent = 0;

    updateConfig(settings->fileName());
}

void Settings::updateConfig(const QString path) {

    settings->sync();

    logH("Updating configuration...");

    // Read in the values
    bool restartTimers = false;

    loadInteger(sLowAlert, lowAlert, 5, 99);
    loadInteger(sHighAlert, highAlert, 6, 100);
    loadInteger(sHealthAlert, healthAlert, 0, 2);
    restartTimers |= loadInteger(sHighNotificationsInterval, highNotificationsInterval, 0, 13);
    restartTimers |= loadInteger(sLowNotificationsInterval, lowNotificationsInterval, 0, 13);
    restartTimers |= loadInteger(sHealthNotificationsInterval, healthNotificationsInterval, 0, 13);
    loadInteger(sLimitEnabled, limitEnabled, 0, 1);
    loadInteger(sLowLimit, lowLimit, 5, 99);
    loadInteger(sHighLimit, highLimit, 6, 100);
    if(loadInteger(sMaxChargeCurrent, maxChargeCurrent, 0, 5000000)) {
        emit setMaxChargeCurrent(maxChargeCurrent);
    }

    notificationTitle = settings->value(sNotificationTitle, notificationTitle).toString();
    notificationLowText = settings->value(sNotificationLowText, notificationLowText).toString();
    notificationHighText = settings->value(sNotificationHighText, notificationHighText).toString();

    notificationHealthTitle    = settings->value(sNotificationHealthTitle, notificationHealthTitle).toString();
    notificationHealthWarnText = settings->value(sNotificationHealthWarnText, notificationHealthWarnText).toString();
    notificationHealthCritText = settings->value(sNotificationHealthCritText, notificationHealthCritText).toString();


    // Update log level
    int oldLogLevel = logLevel;
    loadInteger(sLogLevel, logLevel, 0, 2);
    if(oldLogLevel != logLevel) {
        logger->debug = (logLevel == 2);
        logger->verbose = (logLevel > 1);
        logL(QString("Log level set to %1").arg((logLevel == 0 ? "low" : (logLevel == 1 ? "medium" : "high"))));
    }

    // Let the file system settle...
    QThread::msleep(100);

    if(watcher->files().contains(path)) {
        logH("Config file already on watchlist");
    }
    else {
        watcher->addPath(path);
        logH("Config file added to watchlist");
    }

    if(restartTimers) {
        emit resetTimers();
    }
}

void Settings::setMaxSupportedCurrent(int newCurrent) {
    settings->setValue(sMaxSupportedChargeCurrent, QByteArray::number(newCurrent));
    if(settings->value(sMaxChargeCurrent, QVariant::fromValue(0)).toInt() == 0) {
        settings->setValue(sMaxChargeCurrent, QByteArray::number(newCurrent));
    }
}
