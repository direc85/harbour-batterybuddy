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
    id: logPage

    allowedOrientations: Orientation.Portrait | Orientation.Landscape | Orientation.LandscapeInverted

    SilicaFlickable {
        id: logFlickable
        anchors.fill: parent
        contentHeight: header.height + column.height

        PageHeader {
            id: header
            title: qsTr("View log")
        }

        Column {
            id: column
            anchors {
                top: header.bottom
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width - 2*Theme.horizontalPageMargin
            spacing: Theme.paddingLarge

            ComboBox {
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
                anchors {
                    left: parent.left
                    right: parent.right
                }
                height: updateButton.height

                Column {
                    width: parent.width / 2
                    Button {
                        id: updateButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Update")
                        onClicked: logLabel.updateText()
                    }
                }
                Column {
                    width: parent.width / 2
                    Button {
                        id: daemonStopButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Copy")
                        onClicked: Clipboard.text = logLabel.text
                    }
                }
            }

            /*
            MyLabel {
                id: logLabel
                text: logger.readLogfile(settings.logFilename)
                font.pixelSize: Theme.fontSizeTiny
                function updateText() {
                    logLabel.text = logger.readLogfile(settings.logFilename)
                }
            }
            */
        ColumnView { id: logLabel
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - Theme.horizontalPageMargin
            itemHeight: Math.floor(Theme.itemSizeExtraSmall / 3) // determined by debug output
            clip: true
            model: ListModel {}
            delegate: Component { Label {
                // print height to adjust logLabel.itemHeight
                // on my test system, height is 41, itemSizeExtraSmall is 122 ( 122/3=40, good enough )
                // Component.onCompleted: console.debug("real height:", height, "item height:", logLabel.itemHeight, "theme height:", Theme.itemSizeExtraSmall, "font height:", font.pixelSize)
                clip: true
                text: line
                width: ListView.view.width
                wrapMode: TextEdit.NoWrap
                truncationMode: TruncationMode.Fade
                font.bold:(/%$/.test(text)) // bold if it's a percentage
                font.pixelSize: Theme.fontSizeTiny
                color: {
                    if (/%$/.test(text)) return Theme.primaryColor
                    // TODO: use a more sophisticated matching method:
                    // [HH:MM:HH] Word: value value
                    const token = text.split(" ")[1]
                    switch (token) {
                        case "Charger:":
                            return Theme.highlightFromColor("green", Theme.colorScheme)
                            break;
                        case "State:":
                            return Theme.highlightFromColor("yellow", Theme.colorScheme)
                            break;
                        case "Temperature:":
                            return Theme.highlightFromColor("orange", Theme.colorScheme)
                            break;
                        case "Health:":
                            return Theme.highlightFromColor("lightblue", Theme.colorScheme)
                            break;
                    default:
                        return Theme.secondaryColor
                    }
                    return Theme.secondaryColor
                }
            }
            }
            property string text: ""
            function updateText() {
                logLabel.text = logger.readLogfile(settings.logFilename)
                logLabel.text.split("\n").forEach(
                    function(l) { model.append({ "line": l }); }
                );
            }

        }
        }
    }
}
