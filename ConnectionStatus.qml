import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    height: 30
    width: parent.width
    required property string ip
    property int sendNumber
    property int receiveNumber

    property var colors:[ "#d3d3d3", "#ffffe0", "#90ee90", "#ff0000"]

    enum ConnectionState {
        IDLE,
        CONNECTING,
        CONNECTED,
        FAILED
    }
    property int cntState: ConnectionStatus.IDLE
    property var statusText: ["Ready", "Connecting to ", "Connected to ", "Connected to "]
    RowLayout
    {
        anchors.fill: parent
        Label { Layout.preferredWidth: 10}
        Label {
            id: connectionStatu
            Layout.preferredWidth: 250
        }

        Item { Layout.fillWidth: true }

        Label {
            text: "N(S): " + sendNumber + "  N(R): " + receiveNumber
            Layout.preferredWidth: 200
        }
    }

    states: [
        State {
            name: "IDLE"
            PropertyChanges {
                target: connectionStatu
                text: statusText[0]
            }
            PropertyChanges { target: root; color: colors[0] }
            when: root.cntState === ConnectionStatus.IDLE
        },
        State {
            name: "CONNECTING"
            PropertyChanges {
                target: connectionStatu
                text: statusText[1] + ip
            }
            PropertyChanges { target: root; color: colors[1] }
            when: root.cntState === ConnectionStatus.CONNECTING
        },
        State {
            name: "CONNECTED"
            PropertyChanges {
                target: connectionStatu
                text: statusText[2] + ip
            }
            PropertyChanges { target: root; color: colors[2] }
            when: root.cntState === ConnectionStatus.CONNECTED
        },
        State {
            name: "FAILED"
            PropertyChanges {
                target: connectionStatu
                text: statusText[3] + ip + " failed"
            }
            PropertyChanges { target: root; color: colors[3] }
            when: root.cntState === ConnectionStatus.FAILED
        }
    ]

}

