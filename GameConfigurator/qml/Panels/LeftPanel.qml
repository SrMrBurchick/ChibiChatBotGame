import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons

Item {
    id: root
    anchors.fill: parent
    anchors.leftMargin: 6

    // Text {
    //     text: root.width + "x" + root.height
    // }
    // anchors.horizontalCenter: parent.horizontalCenter

    ColumnLayout {
        anchors.fill: parent

        // Actions list
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 5

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Actions List"
                font.pointSize: 16
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                border.color: "black"
                border.width: 2
                color: "white"

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 4

                    ListView {
                        // TODO: Add c++ model
                        model: ListModel {
                            ListElement {
                                name: "Walk"
                            }
                            ListElement {
                                name: "Climb"
                            }
                            ListElement {
                                name: "Fall"
                            }
                            ListElement {
                                name: "Dance"
                            }
                        }

                        delegate: Rectangle {
                            width: parent.width
                            height: 30

                            Text {
                                text: parent.width + "x" + parent.height + "->" + name
                            }
                        }
                    }
                }
            }

            Button {
                text: "Add action"
                font.pointSize: 14
                onClicked: addAction()
                Layout.fillWidth: true
            }

        }

        // Settings
        ColumnLayout {
            Layout.maximumHeight: 400
            Layout.fillWidth: true

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Settings"
                font.pointSize: 16
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                border.color: "black"
                border.width: 2
                color: "white"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 4

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Table settings"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // TODO: Add c++ model
                        model: ListModel {
                            ListElement {
                                name: "columns"
                                value: 12
                            }
                            ListElement {
                                name: "rows"
                                value: 13
                            }
                        }

                        delegate: Item {
                            Layout.fillWidth: true
                            height: 30

                            RowLayout {
                                anchors.fill: parent

                                Text {
                                    text: name
                                }

                                TextArea {
                                    text: value
                                    Layout.alignment: Qt.AlignRight
                                }
                            }
                        }
                    }

                    Text {
                        text: "Sprite settings"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // TODO: Add c++ model
                        model: ListModel {
                            ListElement {
                                name: "height"
                                value: 32
                            }
                            ListElement {
                                name: "width"
                                value: 32
                            }
                        }

                        delegate: Item {
                            Layout.fillWidth: true
                            height: 30

                            RowLayout {
                                Text {
                                    text: name
                                }

                                TextArea {
                                    text: value
                                    Layout.alignment: Qt.AlignRight
                                }
                            }
                        }
                    }
                }

            }
        }

        // Navigation
        Button {
            text: "Load sprite sheet"
            font.pointSize: 14
            onClicked: loadSpriteSheet()
            Layout.fillWidth: true
        }

        Button {
            text: "Back"
            font.pointSize: 14
            onClicked: goBack()
            Layout.fillWidth: true
        }
    }

    signal addAction()
    signal goBack()
    signal loadSpriteSheet()
}

