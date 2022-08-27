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

BackgroundItem {
    id: root
    property string source

    anchors.horizontalCenter: parent.horizontalCenter
    width: Theme.iconSizeExtraLarge
    height: Theme.iconSizeExtraLarge * (image.baseHeight / image.baseWidth)
    contentItem.radius: Theme.paddingMedium

    Rectangle {
        anchors.fill: parent
        radius: Theme.paddingMedium
        color: Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
        Image {
            id: image
            anchors.centerIn: parent
            source: Qt.resolvedUrl(root.source)
            property real baseWidth: root.width * 0.85 * (sourceSize.width > sourceSize.height ? 1.0 : (sourceSize.width / sourceSize.height))
            property real baseHeight: root.width * 0.85 * (sourceSize.height > sourceSize.width ? 1.0 : (sourceSize.height / sourceSize.width))
            width: baseWidth * (root.down ? 0.95 : 1.0)
            height: baseHeight * (root.down ? 0.95 : 1.0)
            smooth: true
            asynchronous: true
        }
    }
}
