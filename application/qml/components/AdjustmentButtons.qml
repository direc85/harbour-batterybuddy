import QtQuick 2.6
import Sailfish.Silica 1.0

Row {
    property MySlider targetSlider
    property int smallChange
    property int largeChange
    anchors {
        left: parent.left
        right: parent.right
        leftMargin: Theme.paddingLarge
        rightMargin: Theme.paddingLarge
    }
    height: minusFive.height + Theme.paddingLarge

    Column {
        width: parent.width / 4
        Button {
            id: minusFive
            width: parent.width - Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            text: "<<"
            onClicked: {
                targetSlider.changeValue(-largeChange)
                targetSlider.saveTimer.restart()
            }
        }
    }
    Column {
        width: parent.width / 4
        Button {
            id: minusOne
            width: parent.width - Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            text: "<"
            onClicked: {
                targetSlider.changeValue(-smallChange)
                targetSlider.saveTimer.restart()
            }
        }
    }Column {
        width: parent.width / 4
        Button {
            id: plusOne
            width: parent.width - Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            text: ">"
            onClicked: {
                targetSlider.changeValue(smallChange)
                targetSlider.saveTimer.restart()
            }
        }
    }
    Column {
        width: parent.width / 4
        Button {
            id: plusFive
            width: parent.width - Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            text: ">>"
            onClicked: {
                targetSlider.changeValue(largeChange)
                targetSlider.saveTimer.restart()
            }
        }
    }
}
