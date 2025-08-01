import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    HorizontalHeaderView {
        id: horizontalHeader
        anchors.top: parent.top
        anchors.left: parent.left

        syncView: tableView

    }

    TableView {
        id: tableView

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: horizontalHeader.bottom
        anchors.bottom: parent.bottom

        model: APDUModel{}

        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            Text {
                text: display
            }
        }
    }

}

