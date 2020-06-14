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

    // Using 'update' works always; it creates a new notification if the ID doesn't match.
    // notificationtol
    args << "-o" << "update"
         << "-i" << noteID
         << "-I" << "/usr/share/icons/hicolor/128x128/apps/harbour-batterybuddy.png"
         << "-A" << "\"Battery Buddy\"" << title << body << title << body;

    QProcess aplay;
    if(!soundFile.isEmpty()) {
        QStringList aplayArgs;
        aplayArgs << soundFile;
        aplay.start("paplay", aplayArgs);
    }

    QProcess notificationtool;
    notificationtool.start("notificationtool", args);
    notificationtool.waitForFinished();

    QString result(notificationtool.readAll());
    if(!result.isEmpty())
        noteID = result.split(' ').last().trimmed();

    // Playing the sound may take a while, so let's do this as late as possible.
    // Shouldn't matter though, because the minimum delay is 1:00
    // and the sound plays for a few seconds.
    aplay.waitForFinished();
    return;
}

void Notification::close()
{
    if(noteID == "1")
        return;

    QStringList args;
    args << "-o" << "remove" << "-i" << noteID;
    QProcess proc;
    proc.start("notificationtool", args);
    proc.waitForFinished();
    return;
}
