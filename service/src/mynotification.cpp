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

MyNotification::MyNotification(Logger* newLogger, QObject* parent) : QObject(parent)
{
    logger = newLogger;
    notification.setAppName("Battery Buddy");
    // Set this manually, so that the correct icon is used.
    notification.setAppIcon("harbour-batterybuddy");
    playSound = false;
    sound.setAudioRole(QAudio::NotificationRole);
    connect(&sound, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(soundLoadedChanged(QMediaPlayer::MediaStatus)));
}

MyNotification::~MyNotification()
{
    close();
}

void MyNotification::send(QString title, QString body, QString soundFile)
{
    title = title.replace("\"", "\\\"");
    body = body.replace("\"", "\\\"");

    int vol = profile->getRingtoneVolume();
    sound.setVolume(vol);

    playSound = true;
    if(sound.media() != QUrl::fromLocalFile(soundFile)) {
        // Signalled to play()
        sound.setMedia(QUrl::fromLocalFile(soundFile));
    }
    else if (playSound){
        // Must manually trigger play()
        sound.play();
        playSound = false;
    }

    notification.setSummary(title);
    notification.setBody(body);
    notification.setPreviewSummary(title);
    notification.setPreviewBody(body);
    notification.publish();

    return;
}

void MyNotification::close()
{
    notification.close();
    return;
}

void MyNotification::soundLoadedChanged(const QMediaPlayer::MediaStatus newStatus) {
    if(playSound && newStatus == QMediaPlayer::LoadedMedia) {
        sound.play();
        playSound = false;
    }
}
