// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2019-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

import QtQuick 2.6
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.LandscapeMask | Orientation.Portrait

    function formatTemperature(temp) {
        if(Qt.locale().measurementSystem === Locale.ImperialUSSystem) {
            return Math.floor((battery.temperature / 10) * 1.8 + 32) + " °F"
        }
        return Math.floor(battery.temperature / 10) + " °C"
    }
}
