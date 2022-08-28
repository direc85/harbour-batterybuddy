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

CoverBackground {
    id: coverPage

    Column {
        spacing: coverPage.width / 20
        anchors {
            left: coverPage.left
            right: coverPage.right
            verticalCenter: parent.verticalCenter
        }
        BatteryGraph {
            x: coverPage.width * 0.3
            y: coverPage.width * 0.25
            width: 0.35 * coverPage.width
            enableLowBatteryAnimation: coverPage.status === Cover.Active
        }
        CoverLabel {
            id: chargeLabel
            text: "🔋 " + battery.charge + "%"
        }
        CoverLabel {
            text: "🔌 " + Math.floor(battery.current / 1000) + " mA"
        }
        CoverLabel {
            height: text === '🌡️ ' ? 0 : chargeLabel.height
            text: "🌡️ " + (battery.temperature === 0x7FFFFFFF ? '? °C' : formatTemperature(battery.temperature))
        }
    }
}
