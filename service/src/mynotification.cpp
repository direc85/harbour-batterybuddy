// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2020-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

#include "mynotification.h"

MyNotification::MyNotification(QObject* parent) : QObject(parent)
{
    notification.setAppName("Battery Buddy");
    // Set this manually, so that the correct icon is used.
    notification.setAppIcon("harbour-batterybuddy");
}

MyNotification::~MyNotification() { }

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
