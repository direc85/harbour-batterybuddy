import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: infoPage
    anchors.fill: parent

    SilicaFlickable {
        id: infoFlickable
        anchors.fill: parent
        contentHeight: header.height + infoColumn.height

        VerticalScrollDecorator {
            flickable: infoFlickable
        }

        PageHeader {
            id: header
            title: qsTr("More info")
        }

        Column {
            id: infoColumn
            anchors.top: header.bottom
            width: parent.width
            spacing: Theme.paddingMedium

            Label {
                x: Theme.paddingLarge
                text: qsTr("What is Battery Buddy?")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Battery Buddy is a small and simple utility designed to improve the lifetime of the battery.")
                      +" "+ qsTr("It simply monitors the battery level and if it has been charged above the limit, it plays a sound and displays a notification.")
                      +" "+ qsTr("A different sound is played and notification send when the battery has been discharged under the limit.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Why does it matter?")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Lithium-based batteries do not last forever. Every charge and discharge wear out the battery a small amount. This is normal and currently unavoidable.")
                      +" "+ qsTr("A full charge from 0% to 100% stresses the battery more than, say, charging from 25% to 75%.")
                      +" "+ qsTr("Charging and discharging the battery lowers the capacity over time. Small discharge wears out the capacity slower than a deep discharge and a full recharge.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Is it noticable?")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Yes, it is. If you always let the battery drain down to 0% and then charge it to 100%, your battery may not see 500 charge cycles.")
                      +" "+ qsTr("A full charge from 0% to 100% stresses the battery more than, say, charging from 25% to 75%.")
                      +" "+ qsTr("If you only let the battery charge go down to 50% and then charge it to 70%, it could last several thousands of cycles!")
                      +"\n\n"+ qsTr("Please keep in mind that the numbers are very coarse estimates. There are also other variables that affect the life cycle, such as charge current and battery temperature.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Why should I care?")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Every battery produced uses a lot of energy and at least some non-renewable resources.")
                      +" "+ qsTr("By taking care of your devices battery, you don't have to buy a new battery so soon.")
                      +" "+ qsTr("Even better; in some devices the battery is very difficult to replace without braking the device, which means you don't have to pay for the service, either.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("What else can I do?")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Uninstall battery-hungry applications you don't use. Reboot your device every now and then in order to keep it running flawlessly.")
                      +"\n\n"+ qsTr("Use flight mode overnight whenever possible in order to cut power usage down. Powering the device off and on again may use a lot more battery than you think!")
                      +"\n\n"+ qsTr("Don't use your phone in hot or cold environments. In the winter keep the phone in your pocket near your body (but take note of the moisture) and use a handsfree device. Extreme temperatures are bad for the battery.")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("I want to know more!")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                x: Theme.paddingLarge*2
                width: parent.width - x*2;
                wrapMode: Text.Wrap
                text: qsTr("Great! There are a lot of articles to read and papers to study, so click the link to begin!")
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
            }
            Item {
                width: parent.width
                height: lowButton.height
                anchors.left: parent.left
                Button {
                    id: lowButton
                    text: "Google Search"
                    onClicked: Qt.openUrlExternally("https://www.google.com/search?q=How+to+prolong+lithium-based+battery+life%3F")
                    anchors.centerIn: parent
                }

            }
            Item {
                width: parent.width
                height: Theme.paddingMedium
            }
        }
    }
}
