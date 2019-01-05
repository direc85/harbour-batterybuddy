import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("About...")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Battery Buddy")
            }
            DetailItem {
                label: qsTr("Charge level")
                value: battery.charge;
            }
            DetailItem {
                label: qsTr("Charging")
                value: battery.charging ? "yes" : "no";
            }
            Slider {
                id: highSlider
                width: parent.width
                label: qsTr("Upper limit")
                minimumValue: 60
                maximumValue: 99
                stepSize: 1
                value: settings.upperLimit
                valueText: highSlider.value
                onValueChanged: settings.upperLimit = highSlider.value
            }
            Slider {
                id: lowSlider
                width: parent.width
                label: qsTr("Lower limit")
                minimumValue: 10
                maximumValue: 40
                stepSize: 1
                value: settings.lowerLimit
                valueText: lowSlider.value
                onValueChanged: settings.lowerLimit = lowSlider.value
            }
        }
    }
}
