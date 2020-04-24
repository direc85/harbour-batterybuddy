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
import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: batteryGraph
    property real borderSize: width * 0.15
    property real charge: battery.charge
    property bool chargerConnected: battery.chargerConnected
    height: 1.75 * width

    Rectangle {
        id: battTip
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 2 * borderSize
        height: borderSize
        color: "white"
        radius: 0.25 * borderSize
    }
    Rectangle {
        id: battBody
        anchors.top: battTip.verticalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        color: "black"
        border.color: "white"
        border.width: borderSize
        radius: 0.25 * borderSize

        Rectangle {
            id: batteryLevel
            border.color: "black"
            border.width: 0.4 * borderSize
            x: borderSize
            y: parent.height - x - height
            width: parent.width - 2.0 * x
            height: (parent.height - 2.0 * x) * charge / 100.0
            color: charge >= 80 ? "white" : charge >= 50 ? "green" : charge >= 20 ? "orange" : "red"
        }
    }
}
