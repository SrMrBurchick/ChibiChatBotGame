import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import Dialogs
import GameActions
import Base
import ConfigComponent

Item {
    id: root
    anchors.fill: parent
    // anchors.margins: 5

    property Item rootPage: Item
    property bool showTableSettings: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // Actions List
        BasePanel {
            visible: !root.showTableSettings
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                BaseText {
                    id: actions_label
                    Layout.alignment: Qt.AlignTop
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Actions List"
                    font.pixelSize: 24
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
                        onAddNewAction:(newAction) => {
                            console.log("Add new action: ", newAction)
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
            visible: showTableSettings
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                id: settings_column
                anchors.fill: parent

                SettingsList {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

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
                    Layout.fillWidth: true
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                    Layout.bottomMargin: 10
                    text: "Load sprite sheet"
                    onClicked: loadSpriteSheet()
                }
            }
        }

        // Navigation
        ColumnLayout {
            Layout.fillWidth: true

            BaseButton {
                Layout.fillWidth: true
                Layout.margins: 10
                text: root.showTableSettings ? "Hide settings" : "Show settings"
                onClicked: {
                    root.showTableSettings = !root.showTableSettings
                }
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

    Component.onCompleted: {
        root.showTableSettings = !Config.isConfigLoaded()
    }

    signal addAction()
    signal goBack()
    signal loadSpriteSheet()
    signal settingsConfigured()
}
