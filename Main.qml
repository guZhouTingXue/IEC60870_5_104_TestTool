import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 1000
    height: 600
    visible: true
    title: qsTr("Hello World")

    APDUTable {
        anchors.fill: parent
    }


}
