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
import QtQuick 2.6
import Sailfish.Silica 1.0
import "../components"

Page {
    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    // Invisible; used to determine
    // the ColumnView delegate size.
    Label {
        id: metrics
        text: "X"
        font.pixelSize: Theme.fontSizeTiny
        opacity: 0
        enabled: false
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        contentHeight: header.height
                       + flow.height
                       + Theme.paddingLarge
                       + logView.height

        ScrollDecorator {
            flickable: flickable
        }

        PageHeader {
            id: header
            title: qsTr("View log")
        }

        // Layout the ComboBox and the buttons either
        // on top each other (portait) or
        // next to each other (landscape)
        Flow {
            id: flow
            anchors.top: header.bottom
            width: parent.width

            ComboBox {
                width: page.isPortrait ? page.width : page.width / 2
                label: qsTr("Log level")
                currentIndex: 0
                menu: ContextMenu {
                    MenuItem { text: qsTr("Quiet", "Low log setting") }
                    MenuItem { text: qsTr("Verbose", "Medium log setting") }
                    MenuItem { text: qsTr("Debug", "High log setting") }
                }
                onCurrentIndexChanged: settings.logLevel = currentIndex
                Component.onCompleted: currentIndex = settings.logLevel
            }

            Row {
                width: page.isPortrait ? page.width : page.width / 2
                height: updateButton.height

                Column {
                    width: parent.width / 2
                    Button {
                        id: updateButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Update")
                        onClicked: logView.updateText()
                    }
                }
                Column {
                    width: parent.width / 2
                    Button {
                        id: daemonStopButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Copy")
                        onClicked: Clipboard.text = logView.text
                    }
                }
            }
        }

        ColumnView {
            id: logView
            anchors {
                top: flow.bottom
                topMargin: Theme.paddingLarge
                left: parent.left
                right: parent.right
                leftMargin: Theme.horizontalPageMargin
            }
            itemHeight: metrics.height
            clip: true
            model: ListModel {}
            delegate: Component {
                Label {
                    clip: true
                    text: line
                    width: logView.width
                    wrapMode: TextEdit.NoWrap
                    truncationMode: TruncationMode.Fade
                    font.bold: true
                    font.pixelSize: Theme.fontSizeTiny
                    color: {
                        switch (text.split(" ")[1]) {
                        case "Charger:":
                            return Theme.highlightFromColor("yellow", Theme.colorScheme)
                        case "State:":
                            return Theme.highlightFromColor("red", Theme.colorScheme)
                        case "Temperature:":
                            return Theme.highlightFromColor("orange", Theme.colorScheme)
                        case "Health:":
                            return Theme.highlightFromColor("blue", Theme.colorScheme)
                        case "Battery:":
                            return Theme.highlightFromColor("green", Theme.colorScheme)
                        }
                        font.bold = false
                        return Theme.secondaryColor
                    }
                }
            }
            // ColumnView only instantiates and renders delegates
            // as they are about to be shown, so we can
            // simply dump the full log at it.
            Component.onCompleted: updateText()
            property string text: ""
            function updateText() {
                model.clear()
                logView.text = logger.readLogfile(settings.logFilename)
                logView.text.split("\n").forEach(
                    function(str) {
                        model.append({ "line": str })
                    }
                );
            }
        }
    }
}
