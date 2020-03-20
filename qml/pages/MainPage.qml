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
                    label: qsTr("Charger connected:")
                    value: battery.chargerConnected ? qsTr("yes") : qsTr("no")
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
            Item {
                width: parent.width
                height: Theme.paddingLarge
            }
        }
    }
}
