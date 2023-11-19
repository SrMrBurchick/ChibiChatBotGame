import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs
import ConfigComponent
import Base

Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view
    property string spriteSheetPath

    signal goBack()
    signal save()
    signal loadSpriteSheet()

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        nameFilters: ["Image files (*.png *.jpg)"]
        onAccepted: {
            console.log(selectedFile)
            ActionsManager.spriteSheetPath = selectedFile;
            ActionsManager.spriteSheetConfigured = false;
            spriteSheetGrid.clearModel();
            actionsSpriteSequence.clearModel();

            loadSpriteSheet();
        }
    }

    // Main layout
    RowLayout {
        id: main
        anchors.fill: parent
        anchors.margins: 5

        // Left panel
        ColumnLayout {
            Layout.maximumWidth: 400
            BasePanel {
                border.width: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Style.pageBGColor
                anchors.rightMargin: 0

                LeftPanel {
                    rootPage: root
                    onGoBack: {
                        rootStack.pop()
                    }
                    onLoadSpriteSheet: {
                        fileDialog.open();
                    }
                    onAddAction: {
                        actionDialog.open();
                    }
                }
            }
        }

        // Middle panel
        ColumnLayout {
            id: middle_root

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: !ActionsManager.spriteSheetConfigured ? Style.buttonBorderSize : 0

                SpriteSheetGrid {
                    id: spriteSheetGrid
                    visible: ActionsManager.spriteSheetConfigured
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Flickable {
                    anchors.fill: parent
                    visible: !ActionsManager.spriteSheetConfigured
                    clip: true
                    contentWidth: image.width; contentHeight: image.height
                    Image {
                        id: image
                        visible: true
                        source: ActionsManager.spriteSheetPath

                        BaseText {
                            text: "(" + image.sourceSize.width + "x" + image.sourceSize.height + ")"
                        }
                    }
                }
            }

            BasePanel {
                Layout.fillWidth: true
                Layout.minimumHeight: 250
                border.width: 0
                anchors.margins: 0

                ActionSpriteSequence {
                    id: actionsSpriteSequence
                }
            }

        }

        // Right panel
        ColumnLayout {
            Layout.maximumWidth: 400
            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 0

                RightPanel {

                }
            }
        }
    }

    Component.onCompleted: {
        if (GlobalConfig.isConfigLoaded && !ActionsManager.spriteSheetConfigured) {
            ActionsManager.spriteSheetPath = Config.getSpriteSheetPath();
            ActionsManager.spriteSizeWidth = Config.getSpriteWidth()
            ActionsManager.spriteSizeHeight = Config.getSpriteHeight()
            ActionsManager.tableSettingsColumns = Config.getTableColumns()
            ActionsManager.tableSettingsRows = Config.getTableRows()
            ActionsManager.spriteScale = Config.getSpriteScale()
            ActionsManager.spriteSheetConfigured = true
            splitImageToSprites()
        }

        GlobalConfig.isBusy = false
    }

    function splitImageToSprites() {
        spriteSheetGrid.initModel()
    }
}

