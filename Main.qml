import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 1000
    height: 600
    visible: true
    title: qsTr("IEC 60870-5-104 Test Tool")

    APDUTable {
        anchors.fill: parent
    }

    menuBar: MainWindowMenuBar {}
    header: Connection {
        ip: "127.000.000.001"
        port: 2404
    }
    footer: ConnectionStatus {
        ip: "127.000.000.001"
    }
}
