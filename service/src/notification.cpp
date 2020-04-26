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
#include "notification.h"

Notification::Notification(QObject* parent) : QObject(parent)
{

}

Notification::~Notification()
{
    close();
}

void Notification::send(QString title, QString body, QString soundFile)
{
    title = title.replace("\"", "\\\"");
    body = body.replace("\"", "\\\"");

    QStringList args;
    QString command;

    // Using 'update' works always; it creates a new one if needed
    command = QString("notificationtool -o update -i %1 -I /usr/share/icons/hicolor/128x128/apps/harbour-batterybuddy.png -A \"Battery Buddy\" \"%2\" \"%3\" \"%2\" \"%3\"").arg(noteID).arg(title).arg(body);

    args << "-l" << "nemo" << "-c" << command;

    QProcess aplay;
    if(!soundFile.isEmpty()) {
        QStringList aplayArgs;
        aplayArgs << "-l" << "nemo" << "-c" << QString("paplay %1").arg(soundFile);
        aplay.start("runuser", aplayArgs);
        qDebug() << "runuser" << aplayArgs;
    }

    QProcess notificationtool;
    notificationtool.start("runuser", args);
    qDebug() << "runuser" << args;
    notificationtool.waitForFinished();
    aplay.waitForFinished();

    QString result(notificationtool.readAll());
    if(!result.isEmpty())
        noteID = result.split(' ').last().trimmed();
    return;
}

void Notification::close()
{
    if(noteID.compare("1") == 0)
        return;

    QStringList args;
    args << "-o" << "remove" << "-i" << noteID;
    QProcess proc;
    proc.start("runuser", args);
    proc.waitForFinished();
    return;
}
