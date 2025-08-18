import QtQuick
import QtQuick.Controls

//import ming.connection 1.0

ApplicationWindow {
    width: 1000
    height: 600
    visible: true
    title: qsTr("IEC 60870-5-104 Test Tool")

    APDUTable {
        anchors.fill: parent
    }
    CS104Connection {
        id: cs104
        onConnectionEvent: event => { console.log("event:", event)
                                   switch(event)
                                       {
                                        case 0:
                                            connectionState.cntState = ConnectionStatus.CONNECTED;
                                        break;
                                        case 1:
                                            connectionState.cntState = ConnectionStatus.IDLE;
                                        break;
                                        case 4:
                                            connectionState.cntState = ConnectionStatus.FAILED;
                                        break;
                                       }

                           }

    }


    menuBar: MainWindowMenuBar {}
    header: Connection {
        ip: "127.000.000.001"
        port: 2404
        onConnectToServer: {
            cs104.connectToServer()
        }
    }
    footer: ConnectionStatus {
        id: connectionState
        ip: "127.000.000.001"
    }
}
