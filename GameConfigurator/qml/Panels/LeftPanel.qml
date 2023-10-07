import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import Dialogs

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

                ActionsListComponent {
                    id: actionsListComponent
                }
            }

            Button {
                text: "Add action"
                font.pointSize: 14

                AddActionDialog {
                    id: addActionDialog
                    onAddNewAction: {
                        actionsListComponent.addAction(newAction)
                    }
                }

                onClicked: {
                    addActionDialog.open()
                }
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

                SettingsList {

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

