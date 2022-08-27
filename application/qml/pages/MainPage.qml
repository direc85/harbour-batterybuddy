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
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted
    property variant statusText: {
        "idle": qsTr("idle", "Charger plugged in, not using nor charging battery"),
        "discharging": qsTr("discharging", "Charger not plugged in, battery discharging"),
        "charging": qsTr("charging", "Charger plugged in and battery charging"),
        "full": qsTr("full", "Battery fully charged"),
        "empty": qsTr("empty", "Battery fully depleted"),
        "unknown": qsTr("unknown", "Battery not detected, or faulty, or something")
    }
    property variant healthText: {
        "good": qsTr("Good", "Battery is OK"),
        "warm": qsTr("Warm", "Battery is warm"),
        "overheat": qsTr("Overheated", "Battery is very hot"),
        "cool": qsTr("Cool", "Battery is cool"),
        "cold": qsTr("Cold", "Battery is very cold"),
        "unknown": qsTr("unknown", "Battery not detected, or faulty, or something")
    }
    property bool serviceRunning: true

    SystemdUserService {
        id: batteryService
        serviceName: 'harbour-batterybuddy.service'
    }

    Timer {
        id: startupTimer
        interval: 1
        repeat: false
        running: true
        onTriggered: {
            if(logger.debug) logger.log("Startup timer started")
            pageStack.pushAttached(Qt.resolvedUrl("SettingsPage.qml"))
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        id: mainFlickable
        anchors.fill: parent
        contentHeight: flow.height + Theme.horizontalPageMargin

        VerticalScrollDecorator { flickable: mainFlickable }

        PullDownMenu {
            MenuItem {
                text: qsTr("Background", "More to read, background information...")
                onClicked: pageStack.push(Qt.resolvedUrl("InfoPage.qml"))
            }
        }

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Flow {
            id: flow
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: header.height + Math.max(columnOne.heigh, columnTwo.height)

            PageHeader {
                id: header
                title: qsTr("Battery Buddy")
            }
            Column {
                id: columnOne
                width: isPortrait ? parent.width : parent.width / 2
                spacing: Theme.paddingLarge

                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Battery status")
                    color: Theme.highlightColor
                }
                Item {
                    width: parent.width
                    // Rotation: width <==> height
                    height: batteryGraph.width
                    BatteryGraph {
                        id: batteryGraph
                        transformOrigin: Item.Center
                        rotation: 90
                        width: parent.width * 0.2
                        anchors.centerIn: parent
                        enableLowBatteryAnimation: Qt.application.active
                    }
                }
                // Detail column
                Column {
                    width: parent.width
                    spacing: 0

                    MyDetailItem {
                        label: qsTr("Charge:")
                        value: battery.charge + "%"
                    }
                    MyDetailItem {
                        label: qsTr("Current:")
                        value: Math.floor(battery.current / 1000) + " mA"
                    }

                    MyDetailItem {
                        label: qsTr("Charger connected:")
                        value: battery.chargerConnected ? qsTr("yes") : qsTr("no")
                    }
                    MyDetailItem {
                        label: qsTr("State:")
                        value: statusText[battery.state]
                    }
                    MyDetailItem {
                        label: qsTr("Health:")
                        value: healthText[battery.health]
                        visible: value !== "unknown"
                    }
                    MyDetailItem {
                        label: qsTr("Temperature:")
                        value: battery.temperature === 0x7FFFFFFF ? healthText["unknown"] : formatTemperature(battery.temperature)
                        visible: battery.temperature !== 0x7FFFFFFF
                    }
                }
            }
            Column {
                id: columnTwo
                width: isPortrait ? parent.width : parent.width / 2
                spacing: Theme.paddingMedium
                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Background service")
                    color: Theme.highlightColor
                }
                Label {
                    x: Theme.paddingLarge*2
                    width: parent.width - x*2;
                    wrapMode: Text.Wrap
                    text: qsTr("If notifications misbehave or there are problems with charger control, restarting the background service should help.")
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
                Row {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: daemonStartButton.height

                    Column {
                        width: parent.width / 2
                        Button {
                            id: daemonStartButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Start")
                            onClicked: batteryService.startService()
                            enabled: batteryService.serviceState !== "active"
                        }
                    }
                    Column {
                        width: parent.width / 2
                        Button {
                            id: daemonStopButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Stop")
                            onClicked: batteryService.stopService()
                            enabled: batteryService.serviceState === "active"
                        }
                    }
                }
                Label {
                    x: Theme.paddingLarge*2
                    width: parent.width - x*2;
                    wrapMode: Text.Wrap
                    text: qsTr("You can close Battery Buddy when you are done, notifications and charger control will continue working in the background.")
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }
    }
}
