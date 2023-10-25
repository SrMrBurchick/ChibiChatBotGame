import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import Dialogs
import GameActions
import Base

Item {
    id: root
    anchors.fill: parent
    anchors.leftMargin: 6

    property Item rootPage: Item

    ColumnLayout {
        anchors.fill: parent

        // Actions list
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 5

            BaseText {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Actions List"
                // font.pointSize: 16
            }

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ActionsListComponent {
                    id: actionsListComponent
                }
            }

            BaseButton {
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

            BaseText {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Settings"
                font.pointSize: 16
            }

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                SettingsList {
                    onSplitImageToSprites: {
                        rootPage.splitImageToSprites()
                    }
                }

            }
        }

        // Navigation
        BaseButton {
            text: "Load sprite sheet"
            onClicked: loadSpriteSheet()
            Layout.fillWidth: true
        }

        BaseButton {
            text: "Back"
            onClicked: goBack()
            Layout.fillWidth: true
        }
    }

    signal addAction()
    signal goBack()
    signal loadSpriteSheet()
    signal settingsConfigured()
}
