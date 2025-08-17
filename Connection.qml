import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root
    title: "Connection"

    required property string ip
    required property int port

    signal connectToServer(string ip, int port)
    onConnectToServer: (ip, port) => console.debug(ip, port)

    RowLayout {
        anchors.fill: parent

        Label { text: "IP/Hostname:" }
        TextField { id: ipInput
            validator: RegularExpressionValidator {
              regularExpression: /^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))\.){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$/
            }
            inputMask: "000.000.000.000"
            Component.onCompleted: { text = root.ip }
        }

        Label { text: "Port:" }
        TextField {
            id: portInput
            validator: IntValidator { bottom: 0; top: 65535; }
            Component.onCompleted: { text = root.port }
        }
        Button { text: "Connect"
            onClicked: {
                root.ip = ipInput.text
                root.port = parseInt(portInput.text)
                root.connectToServer(root.ip, root.port)
            }
        }
        Item { Layout.fillWidth: true }
    }
}
