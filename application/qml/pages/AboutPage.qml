/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2022 Matti Viljanen
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
import QtQuick 2.6
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
                text: "Copyright 2019-2022 Matti Viljanen (direc85)"
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                text: qsTr("Contributors")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: "Copyright 2021 Peter Gantner (nephros)"
            }

            ImageButton {
                source: Qt.resolvedUrl("/usr/share/harbour-batterybuddy/images/ko-fi.png")
                onClicked: Qt.openUrlExternally("https://ko-fi.com/direc85")
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
                      "Deutsch: dryo, fllp, nephros\n"+
                      "Polski: atlochowski, wetab73\n"+
                      "Magyar: 1Zgp\n"+
                      "Русский": sprainbrains"
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

            ImageButton {
                source: Qt.resolvedUrl(
                            Theme.colorScheme === Theme.LightOnDark
                            ? "/usr/share/harbour-batterybuddy/images/GitHub-Mark-Light-120px-plus.png"
                            : "/usr/share/harbour-batterybuddy/images/GitHub-Mark-120px-plus.png"
                        )
                onClicked: Qt.openUrlExternally("https://github.com/direc85/harbour-batterybuddy")
            }

            AboutLabel {
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: qsTr("A modified version of %1 by %2 in the emoji pack %3 is used in the logo. License: %4")
                    .arg("Battery emoji")
                    .arg("Vincent Le Moign")
                    .arg("Streamline")
                    .arg("Creative Commons Attribution 4.0")
            }

            ImageButton {
                source: Qt.resolvedUrl("/usr/share/harbour-batterybuddy/images/battery-emoji-clipart-original.png")
                onClicked: Qt.openUrlExternally("https://creazilla.com/nodes/49604-battery-emoji-clipart")
            }

            Item {
                width: parent.width
                height: Theme.paddingMedium
            }
        }
    }
}
