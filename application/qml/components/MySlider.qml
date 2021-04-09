import QtQuick 2.2
import Sailfish.Silica 1.0

Slider {
    handleVisible: enabled
    property Timer saveTimer: _saveTimer
    width: parent.width
    function changeValue(amount) {
        var newValue = value + amount
        if(newValue < minimumValue)
            value = minimumValue
        else if(newValue > maximumValue)
            value = maximumValue
        else
            value = newValue
    }
    Timer {
        id: _saveTimer
        interval: 500
        repeat: false
        onTriggered: parent.save()
    }
}
