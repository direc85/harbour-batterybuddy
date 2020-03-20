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
    id: settingsPage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: header.height + settingsColumn.height + Theme.horizontalPageMargin

        PageHeader {
            id: header
            title: qsTr("Settings")
        }

        Column {
            id: settingsColumn
            anchors {
                top: header.bottom
                left: parent.left
                right: parent.right
            }

            spacing: Theme.paddingMedium

            Label {
                x: Theme.paddingLarge
                text: qsTr("Alert settings")
                color: Theme.highlightColor
            }
            MySlider {
                width: parent.width
                label: qsTr("Alert interval")
                minimumValue: 60
                maximumValue: 600
                stepSize: 10
                value: settings.interval
                valueText: Math.floor(value / 60) + (value % 60 < 10 ? ":0" + value % 60 : ":" + value % 60)
                onValueChanged: settings.interval = value
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Set the maximum and minimum target charge levels.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            MySlider {
                id: highAlertSlider
                width: parent.width
                label: qsTr("Charging limit")
                minimumValue: 10
                maximumValue: 99
                stepSize: 1
                value: settings.highAlert
                valueText: value + "%"
                highlightDirection: Qt.RightToLeft
                onValueChanged: {
                    settings.highAlert = value
                    if(lowAlertSlider.value >= value)
                        lowAlertSlider.value = value - 1
                }
            }
            MySlider {
                id: lowAlertSlider
                width: parent.width
                label: qsTr("Discharging limit")
                minimumValue: 10
                maximumValue: 99
                stepSize: 1
                value: settings.lowAlert
                valueText: value + "%"
                onValueChanged: {
                    settings.lowAlert = value
                    if(highAlertSlider.value <= value)
                        highAlertSlider.value = value + 1
                }
            }
            TextSwitch {
                id: autoStopCharging
                text: qsTr("Stop charging when limit reached")
                description: qsTr("This option stops charging when battery has reached the percentage set in Charging limit value, and resumes charging when charge has decreased below Continue charge limit value. Generally a value close to the Charging limit value is recommened, such as 80% and 75%.")
                checked: settings.limitEnabled
                onCheckedChanged: settings.limitEnabled = checked
            }

            MySlider {
                id: highLimitSlider
                handleVisible: enabled
                width: parent.width
                label: qsTr("Stop charging limit")
                minimumValue: 20
                maximumValue: 95
                stepSize: 1
                value: settings.highLimit
                valueText: value + "%"
                highlightDirection: Qt.RightToLeft
                onValueChanged: {
                    settings.highLimit = value
                    if(lowLimitSlider.value >= value)
                        lowLimitSlider.value = value - 1
                }
            }
            MySlider {
                id: lowLimitSlider
                handleVisible: enabled
                width: parent.width
                label: qsTr("Resume charging limit")
                minimumValue: 20
                maximumValue: 95
                stepSize: 1
                value: settings.lowLimit
                valueText: value + "%"
                onValueChanged: {
                    settings.lowLimit = value
                    if(highLimitSlider.value <= value)
                        highLimitSlider.value = value + 1
                }
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("You can also independently stop and resume charging.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Row {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                height: button.height

                Column {
                    width: parent.width / 2
                    Button {
                        id: button
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Enable")
                        onClicked: chargingEnabled = true
                    }
                }
                Column {
                    width: parent.width / 2
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Disable")
                        onClicked: chargingEnabled = false
                    }
                }
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
            Row {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                height: button.height

                Column {
                    width: parent.width / 2
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Charged")
                        onClicked: {
                            alertHigh.play()
                            notification.republishTest()
                        }
                    }
                }
                Column {
                    width: parent.width / 2
                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Discharged")
                        onClicked: {
                            alertLow.play()
                            notification.republishTest()
                        }
                    }
                }
            }
        }
    }
}
