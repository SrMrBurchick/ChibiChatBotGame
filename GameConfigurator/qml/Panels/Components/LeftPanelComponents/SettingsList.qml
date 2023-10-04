import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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

