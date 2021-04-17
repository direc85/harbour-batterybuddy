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
#include "profile.h"

Profile::Profile(Logger* newLogger, QObject *parent) : QObject(parent)
{
    logger = newLogger;
}

uint Profile::getRingtoneVolume() {
    QDBusInterface interface("com.nokia.profiled",
                              "/com/nokia/profiled",
                              "com.nokia.profiled",
                             connection);

    QDBusMessage message = interface.call("get_profile");
    QString profile = message.arguments().at(0).toString();
    logD("Active profile:" + profile);

    if(profile == "silent") {
        logD("Returning volume: 0");
        return 0;
    }

    message = interface.call("get_value", "profile", "ringing.alert.volume");
    int volume = message.arguments().at(0).toInt();
    logD(QString("Ringtone volume: %1").arg(volume));

    return (volume >= 0 && volume <= 100 ? volume : 0);
}
