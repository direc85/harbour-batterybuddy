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
import QtMultimedia 5.6
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import "../components"

Page {
    id: page
    property variant statusText: {
        "idle": qsTr("idle", "Charger plugged in, not using nor charging battery"),
        "discharging": qsTr("discharging", "Charger not plugged in, battery discharging"),
        "charging": qsTr("charging", "Charger plugged in and battery charging"),
        "unknown": qsTr("unknown", "Battery not detected, or faulty, or something")
    }

    onStatusChanged: {
        if(status == PageStatus.Activating) {
            alertTimer.interval = settings.interval * 1000;
        }
    }

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
        interval: settings.interval * 1000 // sec -> msec
        running: true
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

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        id: mainFlickable
        anchors.fill: parent
        contentHeight: column.height

        VerticalScrollDecorator { flickable: mainFlickable }

        PullDownMenu {
            MenuItem {
                text: qsTr("About", "About this application")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Background", "More to read, background information...")
                onClicked: pageStack.push(Qt.resolvedUrl("InfoPage.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Battery Buddy")
            }
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
                    label: qsTr("Charging:")
                    value: battery.charging ? qsTr("yes") : qsTr("no")
                }
                MyDetailItem {
                    label: qsTr("State:")
                    value: statusText[battery.state]
                }
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Please leave Battery Buddy running in the background in order to receive alerts.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Alert tests")
                color: Theme.highlightColor
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Click the buttons to test the sound and notification.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Item {
                width: parent.width
                height: lowButton.height
                anchors.left: parent.left
                Item {
                    width: parent.width / 2
                    height: lowButton.height
                    Button {
                        id: lowButton
                        anchors.centerIn: parent
                        text: qsTr("Discharged")
                        onClicked: {
                            alertLow.play()
                            notification.republishTest()
                        }
                    }
                }
                Item {
                    anchors.right: parent.right
                    width: parent.width / 2
                    height: lowButton.height
                    Button {
                        text: qsTr("Charged")
                        anchors.centerIn: parent
                        onClicked: {
                            alertHigh.play()
                            notification.republishTest()
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: Theme.paddingLarge
            }
        }
    }
}
