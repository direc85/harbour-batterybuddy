// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

#ifndef MYNOTIFICATION_H
#define MYNOTIFICATION_H

#include <QObject>
#include <nemonotifications-qt5/notification.h>

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
    Notification notification;
};

#endif // MYNOTIFICATION_H
