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
#include "mynotification.h"

MyNotification::MyNotification(QObject* parent) : QObject(parent)
{
    notification.setAppName("Battery Buddy");
    // Set this manually, so that the correct icon is used.
    notification.setAppIcon("harbour-batterybuddy");
}

MyNotification::~MyNotification()
{
    notification.close();
}

void MyNotification::send(QString title, QString body, QString soundFile)
{
    title = title.replace("\"", "\\\"");
    body = body.replace("\"", "\\\"");

    notification.setSummary(title);
    notification.setBody(body);
    notification.setPreviewSummary(title);
    notification.setPreviewBody(body);
    notification.setSound(soundFile);
    notification.setUrgency(Notification::Normal);
    notification.publish();

    return;
}

void MyNotification::close()
{
    notification.close();
    return;
}
