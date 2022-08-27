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
import QtQuick 2.6
import Sailfish.Silica 1.0

Item {
    id: batteryGraph
    property real borderSize: width * 0.1
    property bool enableLowBatteryAnimation
    property int _charge: battery.charge

    height: 1.75 * width

//    Timer {
//        id: debugChargeAnimation
//        property int newCharge: -20
//        repeat: true
//        running: true
//        interval: 300
//        onTriggered: {
//            newCharge = newCharge + 5
//            if(newCharge > 120)
//                newCharge = -20
//            _charge = newCharge < 0 ? 0 : newCharge > 100 ? 100 : newCharge
//        }
//    }

    Timer {
        id: lowChargeBlink
        property int counter: 0
        running: (false && enableLowBatteryAnimation
                  && !battery.chargerConnected
                  && _charge <= settings.lowAlert)
        repeat: true
        interval: 400
        onTriggered: {
            counter = (counter + 1) % 5
            if(counter === 0) {
                batteryLevel.opacity = 0.0
                restoreOpacity.start()
            }
        }
    }

    Timer {
        id: restoreOpacity
        running: false
        repeat: false
        interval: opacityAnimation.duration
        onTriggered: batteryLevel.opacity = 1.0
    }

    Rectangle {
        id: battTip
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 2 * borderSize
        height: 1.5 * borderSize
        color: Theme.highlightBackgroundColor
        radius: borderSize / 2
    }
    Rectangle {
        id: battBody
        anchors {
            top: battTip.verticalCenter
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width * 0.75
        color: "transparent"
        border.color: Theme.highlightBackgroundColor
        border.width: borderSize
        radius: borderSize

        Rectangle {
            id: batteryLevel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: borderSize * 1.5
            width: parent.width - 3 * borderSize
            height: (borderSize) + ((parent.height - 4 * borderSize) * _charge / 100.0)
            radius: borderSize / 2
            opacity: 1.0
            color: _charge >= 80 ? Theme.highlightBackgroundColor
                 : _charge >= 50 ? Theme.highlightFromColor("green", Theme.colorScheme)
                 : _charge >= 20 ? Theme.highlightFromColor("yellow", Theme.colorScheme)
                 :                 Theme.highlightFromColor("red", Theme.colorScheme)
            Behavior on color {
                ColorAnimation {}
            }
            Behavior on opacity {
                NumberAnimation {
                    id: opacityAnimation
                }
            }
        }
    }
}
