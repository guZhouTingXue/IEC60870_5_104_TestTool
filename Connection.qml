import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Connection"

    RowLayout {
        anchors.fill: parent

        Label { text: "IP/Hostname:" }
        TextField { id: ipInput
            validator: RegularExpressionValidator {
              regularExpression: /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/
            }
            inputMask: "000.000.000.000;0"
        }

        Label { text: "Port:" }
        TextField {
            id: portInput
            validator: IntValidator { bottom: 0; top: 65535; }
        }
        Button { text: "Connect" }
        Item { Layout.fillWidth: true }
    }
}
