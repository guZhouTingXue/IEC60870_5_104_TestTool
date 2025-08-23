import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root
    title: "Connection"

    required property string ip
    required property int port

    signal connectToServer(string ip, int port)
    signal disconnectToServe()
    onConnectToServer: (ip, port) => console.debug(ip, port)

    enum ConnectionState {
        IDLE,
        CONNECTING,
        CONNECTED
    }
    property int cntState: Connection.IDLE

    states: [
        State {
            name: "IDLE"
            when: root.cntState === Connection.IDLE
            PropertyChanges {
                target: root
                enabled: true
            }
            PropertyChanges {
                target: cntBtn
                text: "Connect"
            }
        },
        State {
            name: "CONNECTING"
            when: root.cntState === Connection.CONNECTING
            PropertyChanges {
                target: root
                enabled: false
            }
            PropertyChanges {
                target: cntBtn
                text: "Connecting"
            }
        },
        State {
            name: "CONNECTED"
            when: root.cntState === Connection.CONNECTED
            PropertyChanges {
                target: root
                enabled: true
            }
            PropertyChanges {
                target: cntBtn
                text: "Disconnect"
            }
        }
    ]

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
        Button {
            id: cntBtn
            text: "Connect"
            onClicked: {
                if(root.cntState === Connection.IDLE)
                {
                    root.cntState = Connection.CONNECTING
                    root.ip = ipInput.text
                    root.port = parseInt(portInput.text)
                    root.connectToServer(root.ip, root.port)
                }
                else
                    root.disconnectToServe()

            }
        }
        Item { Layout.fillWidth: true }
    }
}
