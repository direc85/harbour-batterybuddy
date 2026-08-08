// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2021-2025 Peter G. <sailfish@nephros.org>

import QtQuick 2.6
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted
    property variant statusText: {
        "idle": qsTr("idle", "Charger plugged in, not using nor charging battery"),
        "not charging": qsTr("not charging", "Charger plugged in, not using nor charging battery"),
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
            MenuItem {
                text: qsTr("About", "About this application")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("View log")
                onClicked: pageStack.push(Qt.resolvedUrl("LogPage.qml"))
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

                    DetailItem {
                        label: qsTr("Charge:")
                        value: battery.charge + "%"
                    }
                    DetailItem {
                        property int current: battery.current
                        property int invertSign: settings.invertSign
                        label: qsTr("Current:")
                        value: Math.floor(current * (invertSign || 1) / 1000) + " mA"

                        onCurrentChanged: {
                            if (invertSign == 0 && battery.state == "discharging") {
                                if (current >= 200000) {
                                    settings.invertSign = -1;
                                }
                                else if (current <= -200000) {
                                    settings.invertSign = 1;
                                }
                            }
                        }
                    }

                    DetailItem {
                        property bool connected: (battery.chargerConnected || battery.acConnected)
                        property string chargerType: {
                            if      ( battery.chargerConnected && !battery.acConnected) { return qsTr("USB") }
                            else if (!battery.chargerConnected &&  battery.acConnected) { return qsTr("AC") }
                            else if ( battery.chargerConnected &&  battery.acConnected) { return qsTr("USB") + "/" + qsTr("AC") }
                            return qsTr("unknown", "Charger not detected, or faulty, or something")
                        }
                        label: qsTr("Charger connected:")
                        value: connected ? (qsTr("yes") + " (" + chargerType + ")") : qsTr("no")
                    }
                    DetailItem {
                        label: qsTr("State:")
                        value: statusText[battery.state]
                    }
                    DetailItem {
                        height: battery.state == "charging" ? implicitHeight : 0
                        clip: true
                        label: qsTr("Time to full:")
                        value: Format.formatDuration(battery.timeToFull, Format.Timepoint)
                        visible: battery.timeToFull !== 0x7FFFFFFF

                        Behavior on height {
                            NumberAnimation { }
                        }
                    }
                    DetailItem {
                        label: qsTr("Health:")
                        value: healthText[battery.health]
                        visible: value !== "unknown"
                    }
                    DetailItem {
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
