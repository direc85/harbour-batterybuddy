/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019 Matti Viljanen
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
import Sailfish.Silica 1.0

Page {
    id: aboutPage
    anchors.fill: parent

    PageHeader {
        id: header
        title: qsTr("Battery Buddy")
    }

    Column {
        anchors.top: header.bottom
        width: parent.width
        spacing: Theme.paddingMedium

        Label {
            x: Theme.paddingLarge
            text: qsTr("Author")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "Matti Viljanen"
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Label {
            x: Theme.paddingLarge
            text: qsTr("License")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "GNU General Public Licence 3.0"
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Code and feedback")
            onClicked: Qt.openUrlExternally("http://www.github.com/direc85/harbour-batterybuddy")
        }
        /*
        Label {
            x: Theme.paddingLarge
            text: qsTr("Sounds")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "Sounds were obtained from Notification Sounds and are licensed under Creative Commons Attribution license."
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Upper limit")
            onClicked: Qt.openUrlExternally("https://notificationsounds.com/notification-sounds/unconvinced-569")
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Lower limit")
            onClicked: Qt.openUrlExternally("https://notificationsounds.com/notification-sounds/unsure-566")
        }
        */
    }
}

