import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs
import ConfigComponent
import ActionsManagerComponent
import Base

Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view
    property string spriteSheetPath
    property bool isImageSplited: false;

    signal goBack()
    signal save()

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        nameFilters: ["Image files (*.png *.jpg)"]
        onAccepted: {
            console.log(selectedFile)
            image.source = selectedFile
            Config.saveSpriteSheetPath(selectedFile)
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
                        root.isImageSplited = false
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
                // border.width: !ActionsManager.spriteSheetConfigured ? Style.buttonBorderSize : 0

                SpriteSheetGrid {
                    id: spriteSheetGrid
                    visible: root.isImageSplited

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Flickable {
                    anchors.fill: parent
                    visible: !root.isImageSplited
                    clip: true
                    contentWidth: image.width; contentHeight: image.height
                    Image {
                        id: image
                        visible: true
                        source: Config.getSpriteSheetPath()

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
            anchors.margins: 0

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RightPanel {

                }
            }

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ActionSettingsPanel {

                }
            }

            BaseButton {
                Layout.margins: 10
                Layout.fillWidth: true
                Layout.minimumHeight: 20
                text: "Save"
                onClicked: saveActionsConfig()
            }
        }
    }

    Component.onCompleted: {
        GlobalConfig.isBusy = false
    }

    function splitImageToSprites() {
        root.isImageSplited = true
        spriteSheetGrid.initModel()
    }

    function saveActionsConfig() {
        if (Config.isConfigLoaded() == false) {
            Config.actionsConfigured()
        } else {
            Config.saveConfig()
        }
    }
}

