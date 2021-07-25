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
#ifndef MYNOTIFICATION_H
#define MYNOTIFICATION_H

#include <QObject>
#include <QMediaPlayer>
#include <nemonotifications-qt5/notification.h>
#include "profile.h"
#include "logger.h"

class MyNotification : public QObject
{
    Q_OBJECT

public:
    MyNotification(Logger* newLogger, QObject* parent = nullptr);
    ~MyNotification();

public slots:
    void send(QString title, QString body, QString soundFile);
    void close();

private:
    Notification notification;
    Logger* logger;
    QString noteID = "1";
    QMediaPlayer sound;
    bool playSound;
    Profile* profile;

private slots:
    void soundLoadedChanged(const QMediaPlayer::MediaStatus newStatus);
};

#endif // MYNOTIFICATION_H
