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
    id: settingsPage
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    Timer {
        id: settingsTimer
        interval: 10
        repeat: false
        onTriggered: {
            // The only setting that can change outside this page
            autoStopCharging.checked = settings.limitEnabled
            console.debug("Charger control enabled updated")
        }
    }

    Component.onCompleted: {
        // These we can read only once
        autoStopCharging.checked = settings.limitEnabled
        highLimitSlider.value = settings.highLimit
        lowLimitSlider.value = settings.lowLimit
        notificationsSwitch.checked = settings.notificationsEnabled
        highAlertSlider.value = settings.highAlert
        lowAlertSlider.value = settings.lowAlert
        intervalSlider.value = settings.interval
        console.debug("SettingsPage values updated")
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: flow.height + Theme.horizontalPageMargin

        PullDownMenu {
            MenuItem {
                text: qsTr("About", "About this application")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

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
                title: qsTr("Settings")
            }

            Column {
                id: columnOne
                width: isPortrait ? parent.width : parent.width / 2
                spacing: Theme.paddingMedium

                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Charging settings")
                    color: Theme.highlightColor
                }
                TextSwitch {
                    id: autoStopCharging
                    text: qsTr("Automatic charging control")
                    description: qsTr("This option disables charging automatically when the battery has charged above the pausing percentage and enables it again when the battery has depleted below the resuming percentage.")
                    onCheckedChanged: settings.limitEnabled = checked
                }

                MySlider {
                    id: highLimitSlider
                    handleVisible: enabled
                    width: parent.width
                    label: qsTr("Pause charging limit")
                    minimumValue: 21
                    maximumValue: 95
                    stepSize: 1
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
                    maximumValue: 94
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        settings.lowLimit = value
                        if(highLimitSlider.value <= value)
                            highLimitSlider.value = value + 1
                    }
                }
            }
            Column {
                id: columnTwo
                width: isPortrait ? parent.width : parent.width / 2
                spacing: Theme.paddingMedium
                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Notification settings")
                    color: Theme.highlightColor
                }
                TextSwitch {
                    id: notificationsSwitch
                    text: qsTr("Use notifications")
                    description: qsTr("Display visual and audible notifications about reached battery charge levels, when the battery charge is below or above desired percentage.")
                    onCheckedChanged: settings.notificationsEnabled = checked
                }
                MySlider {
                    id: highAlertSlider
                    width: parent.width
                    label: qsTr("Battery full notification")
                    minimumValue: 11
                    maximumValue: 100
                    stepSize: 1
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
                    label: qsTr("Battery low notification")
                    minimumValue: 10
                    maximumValue: 99
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        settings.lowAlert = value
                        if(highAlertSlider.value <= value)
                            highAlertSlider.value = value + 1
                    }
                }
                MySlider {
                    id: intervalSlider
                    width: parent.width
                    label: qsTr("Notification interval")
                    minimumValue: 60
                    maximumValue: 600
                    stepSize: 10
                    valueText: Math.floor(value / 60) + (value % 60 < 10 ? ":0" + value % 60 : ":" + value % 60)
                    onValueChanged: settings.interval = value
                }
            }
        }
    }
}
