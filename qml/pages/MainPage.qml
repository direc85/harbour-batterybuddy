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

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    MediaPlayer {
        id: alertLow
        audioRole: MediaPlayer.AlarmRole
        volume: 0.5
        autoLoad: true
        source: settings.lowAlertFile
    }

    MediaPlayer {
        id: alertHigh
        audioRole: MediaPlayer.AlarmRole
        volume: 0.5
        autoLoad: true
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
    }

    Timer {
        interval: 60000
        running: true
        repeat: true
        onTriggered: {
            if(battery.charge <= settings.lowerLimit && battery.charging === false) {
                alertLow.play()
                notification.publish()
            }
            else if(battery.charge >= settings.upperLimit && battery.charging === true) {
                alertHigh.play()
                notification.publish()
            }
            else
                notification.close()
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
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Charge level") + " " + battery.charge + "%, " + (battery.charging ? qsTr("charging") : qsTr("discharging"))
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Alert levels")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Set the maximum and minimum target charge levels.")
                      +"\n\n"+ qsTr("Please leave Battery Buddy running in the background in order to receive alerts.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Slider {
                id: highSlider
                width: parent.width
                label: qsTr("Charging limit")
                minimumValue: 60
                maximumValue: 99
                stepSize: 1
                value: settings.upperLimit
                valueText: highSlider.value + "%"
                onValueChanged: settings.upperLimit = highSlider.value
            }
            Slider {
                id: lowSlider
                width: parent.width
                label: qsTr("Discharging limit")
                minimumValue: 10
                maximumValue: 40
                stepSize: 1
                value: settings.lowerLimit
                valueText: lowSlider.value + "%"
                onValueChanged: settings.lowerLimit = lowSlider.value
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Alert tests")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
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
                        text: qsTr("Discharged")
                        onClicked: {
                            alertLow.play()
                            notification.test = true
                            notification.publish()
                            notification.test = false
                        }
                        anchors.centerIn: parent
                    }
                }
                Item {
                    anchors.right: parent.right
                    width: parent.width / 2
                    height: lowButton.height
                    Button {
                        text: qsTr("Charged")
                        onClicked: {
                            alertHigh.play()
                            notification.test = true
                            notification.publish()
                            notification.test = false
                        }
                        anchors.centerIn: parent
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
