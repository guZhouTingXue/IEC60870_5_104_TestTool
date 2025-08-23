import QtQuick
import QtQuick.Controls

//import ming.connection 1.0

ApplicationWindow {
    id: root
    width: 1000
    height: 600
    visible: true
    title: qsTr("IEC 60870-5-104 Test Tool")

    APDUTable {
        anchors.fill: parent
    }
    required property CS104Connection cs104
    Connections {
        target: root.cs104
        function onConnectionEvent(event) {
            console.log("event:", event)
            switch(event) {
                case 0: //OPENED
                    connectionState.cntState = ConnectionStatus.CONNECTED;
                    connection.cntState = Connection.CONNECTED
                break;
                case 1: //CLOSED
                    connectionState.cntState = ConnectionStatus.IDLE;
                    connection.cntState = Connection.IDLE
                break;
                case 4:
                    connectionState.cntState = ConnectionStatus.FAILED;
                    connection.cntState = Connection.IDLE
                break;
            }

        }

    }


    menuBar: MainWindowMenuBar {}
    header: Connection {
        id: connection
        ip: "127.000.000.001"
        port: 2404
        onConnectToServer: {
            cs104.connectToServer()
            connectionState.cntState = ConnectionStatus.CONNECTING
        }
        onDisconnectToServe: {
            cs104.disconnectToServer()
        }
    }
    footer: ConnectionStatus {
        id: connectionState
        ip: "127.000.000.001"
    }
}
