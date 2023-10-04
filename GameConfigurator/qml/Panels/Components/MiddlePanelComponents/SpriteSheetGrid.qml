import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 5

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 3

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Sprite sheet"
            }

            GridView {
                id: spritesGrid
                // TODO: height and width link to image size
                width: 500
                height: 500
                // cellWidth: 80; cellHeight: 80

                // TODO: move model to c++
                model: ListModel {
                    ListElement {
                        column: 0
                        row: 0
                    }
                    ListElement {
                        column: 1
                        row: 0
                    }
                    ListElement {
                        column: 2
                        row: 0
                    }
                    ListElement {
                        column: 3
                        row: 0
                    }

                    ListElement {
                        column: 4
                        row: 0
                    }

                    ListElement {
                        column: 5
                        row: 0
                    }

                    ListElement {
                        column: 0
                        row: 1
                    }

                    ListElement {
                        column: 1
                        row: 1
                    }

                    ListElement {
                        column: 2
                        row: 1
                    }

                    ListElement {
                        column: 3
                        row: 1
                    }

                }

                delegate: Rectangle {
                    // TODO: Move width and height to c++
                    border.color: "black"
                    border.width: 2
                    height: 80
                    width: 80

                    Text {
                        text: "(" + column + " : " + row + ")"
                    }
                }
            }

        }

    }
}

