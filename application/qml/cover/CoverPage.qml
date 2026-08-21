// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>
// SPDX-FileCopyrightText: 2025 Peter G. <sailfish@nephros.org>

import QtQuick 2.6
import Sailfish.Silica 1.0
import "../components"

CoverBackground {
    id: coverPage

    Column {
        spacing: coverPage.width / 20
        anchors {
            left: coverPage.left
            right: coverPage.right
            verticalCenter: parent.verticalCenter
        }
        CoverLabel {
            property bool charging: battery.state == "charging"
            text: "⏲ %1".arg(Format.formatDuration(battery.timeToFull, Format.Timepoint))
            visible: battery.timeToFull !== 0x7FFFFFFF
            height: charging && visible ? implicitHeight : 0
            clip: true

            Behavior on height {
                NumberAnimation { }
            }
        }
        Item {
            width: parent.width
            height: parent.height * 0.3
            BatteryGraph {
                width: parent.height
                height: parent.width * 0.55
                anchors.centerIn: parent
                rotation: 90
                transformOrigin: Item.Center
                enableLowBatteryAnimation: coverPage.status === Cover.Active
            }
        }
        CoverLabel {
            id: chargeLabel
            text: "🔋 " + battery.charge + "%"
        }
        CoverLabel {
            text: "🔌 " + Math.floor(battery.current * (settings.invertSign || 1) / 1000) + " mA"
        }
        CoverLabel {
            height: text === '🌡️ ' ? 0 : chargeLabel.height
            text: "🌡️ " + (battery.temperature === 0x7FFFFFFF ? '? °C' : formatTemperature(battery.temperature))
        }
    }
}
