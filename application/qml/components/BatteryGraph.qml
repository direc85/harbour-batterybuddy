// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2023 Peter G. <sailfish@nephros.org>

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
        running: (enableLowBatteryAnimation
                  && !battery.chargerConnected
                  && !battery.acConnected
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
        color: Theme.secondaryColor
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
        border.color: Theme.secondaryColor
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
            color: _charge >= 80 ? Theme.secondaryColor
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
