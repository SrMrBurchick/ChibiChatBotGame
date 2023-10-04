import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: panel
        anchors.fill: root

        Text {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Sprite sheet"
        }

        Rectangle {
            id: spritesGrid
            anchors.top: title.bottom
            anchors.bottom: panel.bottom

            width: panel.width

            border.color: "black"
            border.width: 2
            color: "white"

            ScrollView {
                clip: true
                anchors.fill: spritesGrid
                anchors.margins: 5

                GridView {
                    anchors.fill: parent
                    // height: panel.height
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

                    delegate: SpriteItemDelegate {

                    }
                }

            }

        }

            // Text {
            //     text: root.width + "x" + root.height
            // }



    }
}

