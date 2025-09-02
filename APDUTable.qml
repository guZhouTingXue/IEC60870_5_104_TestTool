import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    HorizontalHeaderView {
        id: horizontalHeader
        anchors.top: parent.top
        anchors.left: parent.left
        clip: true
        syncView: tableView

    }
    property alias model: tableView.model

    TableView {
        id: tableView
        clip: true
        // interactive: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: horizontalHeader.bottom
        anchors.bottom: parent.bottom

        rowSpacing: 1
        columnSpacing: 1

        model: APDUModel{ }

        selectionModel: ItemSelectionModel {
            id: selection
            model: tableView.model
        }
        selectionBehavior: TableView.SelectRows
        selectionMode: TableView.SingleSelection;
        onCurrentRowChanged: function(){
              console.info("currentRow:", currentRow)
              let currentIndex = model.index(currentRow, currentColumn)
              selection.select(currentIndex,
                               ItemSelectionModel.ClearAndSelect
                               | ItemSelectionModel.Rows
                               );
          }

        delegate: Rectangle {
            clip: true
            implicitWidth: {
                if(column === 10)
                    return 150
                if(column === 11)
                    return 100
                return content.implicitWidth + 40
            }
            implicitHeight: 30
            required property bool current
            required property bool selected
            required property int index
            Label {
                id: content
                text: display
                elide: Text.ElideRight
                anchors.centerIn: parent
                color: selected? "white" : "black"
            }
            color: selected ? "#0078d7" : "white"
            //onSelectedChanged: console.info("index: ", index, " selected:", selected)
        }
        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn

        }
    }

}

