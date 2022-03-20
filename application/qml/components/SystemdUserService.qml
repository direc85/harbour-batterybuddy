import QtQuick 2.6
import Nemo.DBus 2.0

Item {
    id: container
    function queryService() { dbusManager.queryService() }
    function stopService() { dbusManager.stopService() }
    function startService() { dbusManager.startService() }
    function enableService() { dbusManager.unmaskService() }
    function disableService() { dbusManager.maskService() }
    function queryActive() { dbusUnit.queryActiveState() }
    function queryEnabled() { dbusUnit.queryMaskedState() }

    property string serviceName

    property string serviceState: "unknown"
    property string serviceMasked: "unknown"

    Component.onCompleted: {
        dbusManager.queryService()
    }

    DBusInterface {
        id: dbusManager
        service: 'org.freedesktop.systemd1'
        path: '/org/freedesktop/systemd1'
        iface: 'org.freedesktop.systemd1.Manager'

        function reload() {
            call('Reload')
        }
        function queryService() {
            typedCall('GetUnit',
                      { 'type': 's', 'value': serviceName },
                      function(result) {
                          //console.log('GetUnit:', result)
                          dbusUnit.path = result
                          dbusUnit.queryActiveState()
                          dbusUnit.queryMaskedState()
                      },
                      function(error, message) {
                          console.log('GetUnit failed:', error)
                          console.log('GetUnit message:', message)
                      })
        }
        function stopService() {
            typedCall('StopUnit',
                      [
                          { 'type': 's', 'value': serviceName },
                          { 'type': 's', 'value': 'fail' }
                      ],
                      function(result) {
                          //console.log("StopUnit:", result)
                          container.serviceState = "failed"
                      },
                      function(error, message) {
                          console.log("StopUnit failed:", error)
                          console.log("StopUnit message:", message)
                          container.serviceState = "error"
                      })
        }
        function startService() {
            typedCall('StartUnit',
                      [
                          { 'type': 's', 'value': serviceName },
                          { 'type': 's', 'value': 'fail' }
                      ],
                      function(result) {
                          //console.log("StartUnit:", result)
                          container.serviceState = "active"
                      },
                      function(error, message) {
                          console.log("StartUnit failed:", error)
                          console.log("StartUnit message:", message)
                          container.serviceState = "error"
                      })
        }
        function unmaskService() {
            typedCall('UnmaskUnitFiles',
                      [
                          { 'type': 'as', 'value': [serviceName] },
                          { 'type': 'b', 'value': false },
                      ],
                      function(install, changes) {
                          //console.log("UnmaskUnitFiles result:", install)
                          //console.log("UnmaskUnitFiles changes:", changes)
                          container.serviceMasked = "loaded"
                          reload()
                      },
                      function(error, message) {
                          console.log("UnmaskUnitFiles failed:", error)
                          console.log("UnmaskUnitFiles message:", message)
                          container.serviceMasked = "error"
                      })
        }
        function maskService() {
            typedCall('MaskUnitFiles',
                      [
                          { 'type': 'as', 'value': [serviceName] },
                          { 'type': 'b', 'value': false },
                          { 'type': 'b', 'value': true },
                      ],
                      function(install, changes) {
                          //console.log("MaskUnitFiles result:", install)
                          //console.log("MaskUnitFiles changes:", changes)
                          container.serviceMasked = "masked"
                          reload()
                      },
                      function(error, message) {
                          console.log("MaskUnitFiles failed:", error)
                          console.log("MaskUnitFiles message:", message)
                          container.serviceMasked = "error"
                      })
        }
    }

    DBusInterface {
        id: dbusUnit
        service: 'org.freedesktop.systemd1'
        path: '/org/freedesktop/systemd1/unit/harbour_2dbatterybuddy_2eservice'
        iface: 'org.freedesktop.systemd1.Unit'

        function queryActiveState() {
            var result = getProperty('ActiveState')
            //console.log('ActiveState:', result)
            container.serviceState = result
        }
        function queryMaskedState() {
            var result = getProperty('LoadState')
            //console.log('LoadState:', result)
            container.serviceMasked = result
        }
    }
}
