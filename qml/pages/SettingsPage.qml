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
    id: settingsPage
    anchors.fill: parent

    PageHeader {
        id: header
        title: qsTr("Settings")
    }

    Column {
        anchors.top: header.bottom
        width: parent.width
        spacing: Theme.paddingMedium

        Label {
            x: Theme.paddingLarge
            text: qsTr("Alert settings")
            color: Theme.highlightColor
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: qsTr("Set the maximum and minimum target charge levels.")
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }
        Slider {
            width: parent.width
            label: qsTr("Charging limit")
            minimumValue: 60
            maximumValue: 99
            stepSize: 1
            value: settings.upperLimit
            valueText: value + "%"
            onValueChanged: settings.upperLimit = value
        }
        Slider {
            width: parent.width
            label: qsTr("Discharging limit")
            minimumValue: 10
            maximumValue: 40
            stepSize: 1
            value: settings.lowerLimit
            valueText: value + "%"
            onValueChanged: settings.lowerLimit = value
        }
        Slider {
            width: parent.width
            label: qsTr("Alert interval")
            minimumValue: 60
            maximumValue: 600
            stepSize: 10
            value: settings.interval
            valueText: Math.floor(value / 60) + (value % 60 < 10 ? ":0" + value % 60 : ":" + value % 60)
            onValueChanged: settings.interval = value
        }
    }
}

