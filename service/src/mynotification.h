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
#include <QProcess>
#include <QTimer>
#include <QSoundEffect>
#include <nemonotifications-qt5/notification.h>
#include <QDebug>

class MyNotification : public QObject
{
    Q_OBJECT

public:
    MyNotification(QObject* parent = nullptr);
    ~MyNotification();

public slots:
    void send(QString title, QString body, QString soundFile);
    void close();

private:
    QString noteID = "1";
    Notification notification;
    QSoundEffect sound;
    bool playSound;

private slots:
    void soundLoadedChanged();
};

#endif // MYNOTIFICATION_H
