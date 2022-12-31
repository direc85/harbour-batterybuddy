/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2022 Matti Viljanen
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
    id: settingsPage
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    SystemdUserService {
        id: batteryService
        serviceName: 'harbour-batterybuddy.service'
        onServiceMaskedChanged: {
            daemonEnabledSwitch.busy = false
        }
    }

    //////////////////////////////////////////////////
    //
    //   TIMERS AND PROCESSES
    //
    //////////////////////////////////////////////////

    Timer {
        id: startupTimer
        interval: 100
        repeat: false
        running: true
        onTriggered: {
            autoStopCharging.checked = settings.limitEnabled
            maxChargeCurrentSlider.value = settings.maxChargeCurrent
            highLimitSlider.value = settings.highLimit
            lowLimitSlider.value = settings.lowLimit
            highAlertSlider.value = settings.highAlert
            healthSelector.currentIndex = settings.healthAlert
            lowAlertSlider.value = settings.lowAlert
            highIntervalCombo.currentIndex = settings.highNotificationsInterval
            lowIntervalCombo.currentIndex = settings.lowNotificationsInterval
            healthIntervalCombo.currentIndex = settings.healthNotificationsInterval
            if(logger.debug) logger.log("SettingsPage values updated")
            batteryService.queryEnabled()
        }
    }

    //////////////////////////////////////////////////
    //
    //   MAIN CONTENT
    //
    //////////////////////////////////////////////////

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: flow.height + Theme.horizontalPageMargin

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

            //////////////////////////////////////////////////
            //
            //   THE LEFT/TOP COLUMN
            //
            //////////////////////////////////////////////////

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
                    checked: batteryService.serviceMasked === "loaded"
                    onClicked: {
                        if(checked)
                            batteryService.enableService()
                        else
                            batteryService.disableService()
                        busy = true
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

                SectionHeader { text: qsTr("Pause charging limit") }

                MySlider {
                    id: highLimitSlider
                    minimumValue: 6
                    maximumValue: 100
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        if(lowLimitSlider.value >= value)
                            lowLimitSlider.value = value - 1
                    }
                    onReleased: save()
                    function save() {
                        settings.lowLimit = lowLimitSlider.value
                        settings.highLimit = value
                    }
                }

                AdjustmentButtons {
                    targetSlider: highLimitSlider
                    smallChange: 1
                    largeChange: 5
                }

                SectionHeader { text: qsTr("Resume charging limit") }

                MySlider {
                    id: lowLimitSlider
                    minimumValue: 5
                    maximumValue: 99
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        if(highLimitSlider.value <= value)
                            highLimitSlider.value = value + 1
                    }
                    onReleased: save()
                    function save() {
                        settings.lowLimit = value
                        settings.highLimit = highLimitSlider.value
                    }
                }

                AdjustmentButtons {
                    targetSlider: lowLimitSlider
                    smallChange: 1
                    largeChange: 5
                }

                SectionHeader {
                    text: qsTr("Maximum Charge Current")
                    visible: settings.maxSupportedChargeCurrent > 0
                }

                MySlider {
                    id: maxChargeCurrentSlider
                    visible: settings.maxSupportedChargeCurrent > 0
                    minimumValue:  500000
                    maximumValue: settings.maxSupportedChargeCurrent
                    stepSize: 100000
                    valueText: (value / 1000) + "mA"
                    onReleased: save()
                    function save() {
                        settings.maxChargeCurrent = value
                    }
                }
            }

            //////////////////////////////////////////////////
            //
            //   THE RIGHT/BOTTOM COLUMN
            //
            //////////////////////////////////////////////////

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

                SectionHeader { text: qsTr("Battery full notification") }

                MySlider {
                    id: highAlertSlider
                    minimumValue: 6
                    maximumValue: 100
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        if(lowAlertSlider.value >= value)
                            lowAlertSlider.value = value - 1
                    }
                    onReleased: save()
                    function save() {
                        settings.lowAlert = lowAlertSlider.value
                        settings.highAlert = value
                    }
                }

                AdjustmentButtons {
                    targetSlider: highAlertSlider
                    smallChange: 1
                    largeChange: 5
                }

                SectionHeader { text: qsTr("Battery low notification") }

                MySlider {
                    id: lowAlertSlider
                    minimumValue: 5
                    maximumValue: 99
                    stepSize: 1
                    valueText: value + "%"
                    onValueChanged: {
                        if(highAlertSlider.value <= value)
                            highAlertSlider.value = value + 1
                    }
                    onReleased: save()
                    function save(){
                        settings.lowAlert = value
                        settings.highAlert = highAlertSlider.value
                    }
                }

                AdjustmentButtons {
                    targetSlider: lowAlertSlider
                    smallChange: 1
                    largeChange: 5
                }

                SectionHeader { text: qsTr("Battery high notification interval") }

                ComboBox {
                    id: highIntervalCombo
                    label: qsTr("Battery high notification interval")
                    menu: ContextMenu {
                        Repeater {
                            model: frequencyNames
                            MenuItem {
                                enabled: index > 0
                                visible: index > 0
                                text: modelData
                                onClicked: {
                                    highIntervalCombo.currentIndex = index
                                    highIntervalCombo.save()
                                }
                            }
                        }
                    }
                    function save() {
                        settings.highNotificationsInterval = currentIndex
                    }
                }

                SectionHeader { text: qsTr("Battery low notification interval") }

                ComboBox {
                    id: lowIntervalCombo
                    label: qsTr("Battery low notification interval")
                    menu: ContextMenu {
                        Repeater {
                            model: frequencyNames
                            MenuItem {
                                enabled: index > 0
                                visible: index > 0
                                text: modelData
                                onClicked: {
                                    lowIntervalCombo.currentIndex = index
                                    lowIntervalCombo.save()
                                }
                            }
                        }
                    }
                    function save() {
                        settings.lowNotificationsInterval = currentIndex
                    }
                }

                Label {
                    x: Theme.paddingLarge
                    text: qsTr("Health notification settings")
                    color: Theme.highlightColor
                }
                Label {
                    text: qsTr("Display visual and audible notifications when the battery status exceeds safe values.<br />This usually means high (or low) temperature but can include other parameters depending on the hardware.")
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

                SectionHeader { text: qsTr("Battery health notification") }

                ComboBox {
                    id: healthSelector
                    width: parent.width
                    label: qsTr("Notification threshold")
                    currentIndex: settings.healthAlert
                    menu: ContextMenu {
                        MenuItem { text: qsTr("Never") }
                        MenuItem { text: qsTr("Warning") }
                        MenuItem { text: qsTr("Critical") }
                    }
                    onValueChanged: save()
                    function save() {
                        settings.healthAlert = healthSelector.currentIndex
                    }
                }

                SectionHeader { text: qsTr("Health notification interval") }

                ComboBox {
                    id: healthIntervalCombo
                    label: qsTr("Health notification interval")
                    menu: ContextMenu {
                        Repeater {
                            model: frequencyNames
                            MenuItem {
                                enabled: index > 0
                                visible: index > 0
                                text: modelData
                                onClicked: {
                                    healthIntervalCombo.currentIndex = index
                                    healthIntervalCombo.save()
                                }
                            }
                        }
                    }
                    function save() {
                        settings.healthNotificationsInterval = currentIndex
                    }
                }
            }
        }
    }
}
