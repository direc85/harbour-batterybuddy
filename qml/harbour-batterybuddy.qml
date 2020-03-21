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
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
import QtQuick 2.0
import QtMultimedia 5.6
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import "pages"

ApplicationWindow
{
    SoundEffect {
        id: alertLow
        volume: 0.6
        source: settings.lowAlertFile

    }

    SoundEffect {
        id: alertHigh
        volume: 0.6
        source: settings.highAlertFile
    }

    Notification {
        id: notification
        property bool test: false
        appName: qsTr("Battery Buddy")
        appIcon: "/usr/share/icons/hicolor/128x128/apps/harbour-batterybuddy.png"
        summary: qsTr("Battery charge", "Battery charge 20%") +" "+ battery.charge + "%"
        body: test ? qsTr("This is a test.") : battery.charging ? qsTr("Please disconnect the charger.") : qsTr("Please connect the charger.")
        previewSummary: summary
        previewBody: body
        urgency: Notification.Critical
        function republish() {
            if(replacesId > 0)
                close()
            publish()
        }
        function republishTest() {
            test = true
            republish()
            test = false
        }
    }

    Timer {
        id: alertTimer
        interval: settings.interval * 1000
        running: settings.notificationsEnabled
        repeat: true
        onTriggered: {
            if(battery.charge <= settings.lowerLimit && battery.state === "discharging") {
                alertLow.play()
                notification.republish()
            }
            else if(battery.charge >= settings.upperLimit &&
                    (battery.state === "charging" && battery.charging === true) || (battery.state === "idle" && battery.charging === false)) {
                alertHigh.play()
                notification.republish()
            }
            else if(notification.replacesId > 0) {
                notification.close()
            }
        }
    }

    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.LandscapeMask | Orientation.Portrait
}
