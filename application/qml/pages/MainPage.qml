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
    property bool settingsPagePushed: false

    onStatusChanged: {
        if(status == PageStatus.Active && !settingsPagePushed) {
            pageStack.pushAttached(Qt.resolvedUrl("SettingsPage.qml"))
            settingsPagePushed = true;
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
                        label: qsTr("Charger connected:")
                        value: battery.chargerConnected ? qsTr("yes") : qsTr("no")
                    }
                    MyDetailItem {
                        label: qsTr("State:")
                        value: statusText[battery.state]
                    }
                }
            }
            Column {
                id: columnTwo
                width: isPortrait ? parent.width : parent.width / 2
                spacing: Theme.paddingMedium
                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Charger control")
                    color: Theme.highlightColor
                }
                Label {
                    x: Theme.paddingLarge*2
                    width: parent.width - x*2;
                    wrapMode: Text.Wrap
                    text: qsTr("Using these controls overrides the automated settings.")
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
                Row {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: resumeButton.height

                    Column {
                        width: parent.width / 2
                        Button {
                            id: resumeButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Resume")
                            onClicked: {
                                battery.chargingEnabled = true
                                settings.limitEnabled = false
                            }
                            enabled: !battery.chargingEnabled
                        }
                    }
                    Column {
                        width: parent.width / 2
                        Button {
                            id: pauseButton
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Pause")
                            onClicked: {
                                battery.chargingEnabled = false
                                settings.limitEnabled = false
                            }
                            enabled: battery.chargingEnabled
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
