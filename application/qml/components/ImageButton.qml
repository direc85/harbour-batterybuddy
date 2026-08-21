// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022-2026 Matti Viljanen <matti.viljanen@kapsi.fi>

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
