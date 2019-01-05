import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage
    anchors.fill: parent

    PageHeader {
        id: header
        title: qsTr("Battery Buddy")
    }

    Column {
        anchors.top: header.bottom
        width: parent.width
        spacing: Theme.paddingMedium

        Label {
            x: Theme.paddingLarge
            text: qsTr("Author")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "Matti Viljanen"
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Label {
            x: Theme.paddingLarge
            text: qsTr("License")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "GNU General Public Licence 3.0"
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Code and feedback")
            onClicked: Qt.openUrlExternally("http://www.github.com/direc85/harbour-countme2")
        }
        Label {
            x: Theme.paddingLarge
            text: qsTr("Sounds")
            color: Theme.highlightColor

            font.pixelSize: Theme.fontSizeLarge
        }
        Label {
            x: Theme.paddingLarge*2
            width: parent.width - x*2;
            wrapMode: Text.Wrap
            text: "Sounds were obtained from Notification Sounds and are licensed under Creative Commons Attribution license."
            color: Theme.primaryColor

            font.pixelSize: Theme.fontSizeMedium
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Upper limit")
            onClicked: Qt.openUrlExternally("https://notificationsounds.com/notification-sounds/unconvinced-569")
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Lower limit")
            onClicked: Qt.openUrlExternally("https://notificationsounds.com/notification-sounds/unsure-566")
        }
    }
}

