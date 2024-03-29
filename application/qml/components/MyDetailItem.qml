/****************************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Joona Petrell <joona.petrell@jollamobile.com>
** All rights reserved.
**
** This file is part of Sailfish Silica UI component package.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Jolla Ltd nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

// Modified by Matti Viljanen 2020: added text size control

import QtQuick 2.6
import Sailfish.Silica 1.0

Item {
    id: detailItem
    width: parent.width
    height: Math.max(labelText.height, valueText.height) + 2*Theme.paddingSmall

    property alias label: labelText.text
    property alias value: valueText.text
    property real leftMargin: Theme.horizontalPageMargin
    property real rightMargin: Theme.horizontalPageMargin
    property int pixelSize: Theme.fontSizeMedium

    Text {
        id: labelText

        y: Theme.paddingSmall
        anchors {
            left: parent.left
            right: parent.horizontalCenter
            rightMargin: Theme.paddingSmall
            leftMargin: detailItem.leftMargin
            verticalCenter: parent.verticalCenter
        }
        horizontalAlignment: Text.AlignRight
        color: Theme.secondaryHighlightColor
        font.pixelSize: detailItem.pixelSize
        textFormat: Text.PlainText
        wrapMode: Text.Wrap
    }

    Text {
        id: valueText

        y: Theme.paddingSmall
        anchors {
            left: parent.horizontalCenter
            right: parent.right
            leftMargin: Theme.paddingSmall
            rightMargin: detailItem.rightMargin
            verticalCenter: parent.verticalCenter
        }
        horizontalAlignment: Text.AlignLeft
        color: Theme.highlightColor
        font.pixelSize: detailItem.pixelSize
        textFormat: Text.PlainText
        wrapMode: Text.Wrap
    }
}
