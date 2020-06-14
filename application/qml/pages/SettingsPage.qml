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
import Process 1.0
import "../components"

Page {
    id: settingsPage
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    Component.onCompleted: {
    }

    Timer {
        id: startupTimer
        interval: 100
        repeat: false
        running: true
        onTriggered: {
            autoStopCharging.checked = settings.limitEnabled
            highLimitSlider.value = settings.highLimit
            lowLimitSlider.value = settings.lowLimit
            highNotificationsSwitch.checked = settings.highNotificationsEnabled
            lowNotificationsSwitch.checked = settings.lowNotificationsEnabled
            highAlertSlider.value = settings.highAlert
            lowAlertSlider.value = settings.lowAlert
            highIntervalSlider.value = settings.highNotificationsInterval
            lowIntervalSlider.value = settings.lowNotificationsInterval
            console.debug("SettingsPage values updated")
            daemonCheck.start()
        }
    }

    Timer {
        id: daemonToggle
        interval: 100
        running: false
        repeat: false
        onTriggered: {
            var action = daemonEnabledSwitch.checked ? "disable" : "enable"
            console.log("Action: " + action)
            _toggleProcess.start("/bin/systemctl", ["--user", action, "harbour-batterybuddy.service"])
        }
    }

    Process {
        // Only used by daemonToggle timer
        id: _toggleProcess
        onFinished: {
            daemonCheck.start()
            console.debug("Service toggle " + (errorCode() === 0 ? "succeeded" : "failed"))
        }
    }

    Timer {
        id: daemonCheck
        interval: 0
        running: false
        repeat: false
        onTriggered: {
            _checkProcess.start("/bin/systemctl", ["--user", "is-enabled", "harbour-batterybuddy.service"])
        }
    }

    Process {
        // Only used by daemonCheck timer
        id: _checkProcess
        onFinished: {
            if(errorCode() === 0) {
                daemonEnabledSwitch.checked = true
            }
            else {
                daemonEnabledSwitch.checked = false
            }
            daemonEnabledSwitch.busy = false
            console.info("Service is " + (errorCode() === 0 ? "enabled" : "disabled"))
        }
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
                    text: qsTr("Background service")
                    color: Theme.highlightColor
                }
                TextSwitch {
                    id: daemonEnabledSwitch
                    text: qsTr("Start background service at startup")
                    checked: true
                    onClicked: {
                        busy = true
                        checked = !checked
                        daemonToggle.start()
                    }
                }
                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Charging settings")
                    color: Theme.highlightColor
                }
                Label {
                    text: qsTr("This option disables charging automatically when the battery has charged above the pausing percentage and enables it again when the battery has depleted below the resuming percentage.")
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: Theme.horizontalPageMargin*2
                        rightMargin: Theme.horizontalPageMargin
                    }
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.Wrap
                }
                TextSwitch {
                    id: autoStopCharging
                    text: qsTr("Automatic charging control")
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
                        if(lowLimitSlider.value >= value)
                            lowLimitSlider.value = value - 1
                    }
                    onReleased: {
                        settings.lowLimit = lowLimitSlider.value
                        settings.highLimit = value
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
                        if(highLimitSlider.value <= value)
                            highLimitSlider.value = value + 1
                    }
                    onReleased: {
                        settings.lowLimit = value
                        settings.highLimit = highLimitSlider.value
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
                Label {
                    text: qsTr("Display visual and audible notifications about reached battery charge levels, when the battery charge is below or above desired percentage.")
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: Theme.horizontalPageMargin*2
                        rightMargin: Theme.horizontalPageMargin
                    }
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.Wrap
                }

                TextSwitch {
                    id: highNotificationsSwitch
                    text: qsTr("Show high charge notification")
                    onCheckedChanged: settings.highNotificationsEnabled = checked
                }
                TextSwitch {
                    id: lowNotificationsSwitch
                    text: qsTr("Show low charge notification")
                    onCheckedChanged: settings.lowNotificationsEnabled = checked
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
                        if(lowAlertSlider.value >= value)
                            lowAlertSlider.value = value - 1
                    }
                    onReleased: {
                        settings.lowAlert = lowAlertSlider.value
                        settings.highAlert = value
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
                        if(highAlertSlider.value <= value)
                            highAlertSlider.value = value + 1
                    }
                    onReleased: {
                        settings.lowAlert = value
                        settings.highAlert = highAlertSlider.value
                    }
                }
                MySlider {
                    id: highIntervalSlider
                    width: parent.width
                    label: qsTr("Battery high notification interval")
                    minimumValue: 60
                    maximumValue: 600
                    stepSize: 10
                    valueText: Math.floor(value / 60) + (value % 60 < 10 ? ":0" + value % 60 : ":" + value % 60)
                    onReleased: settings.highNotificationsInterval = value
                }
                MySlider {
                    id: lowIntervalSlider
                    width: parent.width
                    label: qsTr("Battery low notification interval")
                    minimumValue: 60
                    maximumValue: 600
                    stepSize: 10
                    valueText: Math.floor(value / 60) + (value % 60 < 10 ? ":0" + value % 60 : ":" + value % 60)
                    onReleased: settings.lowNotificationsInterval = value
                }
            }
        }
    }
}
