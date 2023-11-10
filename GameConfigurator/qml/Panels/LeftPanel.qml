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
    // anchors.margins: 5

    property Item rootPage: Item

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // Actions List
        BasePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                BaseText {
                    id: actions_label
                    Layout.alignment: Qt.AlignTop
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Actions List"
                    font.pixelSize: 32
                }
                ActionsListComponent {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    id: actionsListComponent
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
                        addActionDialog.setupDialog()
                        addActionDialog.open()
                    }
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Layout.margins: 10
                }
            }
        }

        // Settings
        BasePanel {
            // Layout.fillHeight: true
            Layout.fillWidth: true
            height: settings_column.height

            ColumnLayout {
                id: settings_column
                width: parent.width

                SettingsList {
                    Layout.fillWidth: true
                }
            }
        }

        // Navigation
        ColumnLayout {
            Layout.fillWidth: true

            BaseButton {
                Layout.fillWidth: true
                Layout.margins: 10
                text: "Split to sprites"
                onClicked: {
                    ActionsManager.spriteSheetConfigured = true
                    rootPage.splitImageToSprites()
                }
            }

            BaseButton {
                Layout.bottomMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                text: "Load sprite sheet"
                onClicked: loadSpriteSheet()
                Layout.fillWidth: true
            }

            BaseButton {
                Layout.bottomMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                text: "Back"
                onClicked: goBack()
                Layout.fillWidth: true
            }
        }
    }

    signal addAction()
    signal goBack()
    signal loadSpriteSheet()
    signal settingsConfigured()
}
