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
import QtQuick 2.2
import Sailfish.Silica 1.0
import "../components"

Page {
    id: aboutPage

    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    onStatusChanged: {
        if(status === PageStatus.Active)
            pageStack.pushAttached(Qt.resolvedUrl("LicensePage.qml"))
    }

    SilicaFlickable {
        id: aboutFlickable
        anchors.fill: parent
        contentHeight: header.height + column.height

        PageHeader {
            id: header
            title: qsTr("View License")
        }

        Column {
            id: column
            anchors {
                top: header.bottom
                horizontalCenter: parent.horizontalCenter
            }
            width: Math.min(Screen.width, aboutFlickable.width)
            spacing: Theme.paddingLarge

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: Qt.resolvedUrl("/usr/share/icons/hicolor/172x172/apps/harbour-batterybuddy.png")
                width: Theme.iconSizeExtraLarge
                height: Theme.iconSizeExtraLarge
                smooth: true
                asynchronous: true
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.highlightColor
                text: "Battery Buddy"
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.highlightColor
                text: app_version
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                text: qsTr("Battery Buddy is a small and simple utility designed to improve the lifetime of the battery.")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: "Copyright 2019-2021 Matti Viljanen (direc85)"
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                text: qsTr("Contributors")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: "Peter Gantner (nephros)"
            }

            BackgroundItem {
                anchors.horizontalCenter: parent.horizontalCenter
                width: Theme.iconSizeExtraLarge * 1.2
                height: Theme.iconSizeExtraLarge * 1.2
                onClicked: Qt.openUrlExternally("https://ko-fi.com/direc85")
                contentItem.radius: Theme.paddingSmall

                Image {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("/usr/share/harbour-batterybuddy/images/ko-fi.png")
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    smooth: true
                    asynchronous: true
                }
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: qsTr("If you like my work and would like to support me, you can buy me a coffee!")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                text: qsTr("Translations")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: "English: direc85"+
                      "Suomi: direc85\n"+
                      "Français: Quenty31\n"+
                      "简体中文: dashinfantry\n"+
                      "svensk: eson57\n"+
                      "Deutsch: dryo, fllp\n"+
                      "Polski: atlochowski\n"+
                      "Magyar: 1Zgp"
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: qsTr("Battery Buddy is open source software licensed under the terms of the GNU General Public License.")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: qsTr("Contributions and translations are always appreciated!")
            }

            Button {
                text: "GitHub"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: Qt.openUrlExternally("https://github.com/direc85/harbour-batterybuddy")
            }

            Item {
                width: parent.width
                height: Theme.paddingMedium
            }
        }
    }
}
