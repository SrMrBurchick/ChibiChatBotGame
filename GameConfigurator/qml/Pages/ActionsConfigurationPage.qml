import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs

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
            ActionsManager.spriteSheetPath = selectedFile
            loadSpriteSheet()
        }
    }

    // Main layout
    RowLayout {
        id: main
        anchors.fill: parent

        // Left panel
        ColumnLayout {
            Layout.maximumWidth: 300
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                AddActionDialog {
                    id: actionDialog
                }

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

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

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

                        Text {
                            text: "(" + image.sourceSize.width + "x" + image.sourceSize.height + ")"
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 200
                border.color: "black"
                border.width: 2
                color: "white"

                ActionSpriteSequence {

                }
            }

        }

        // Right panel
        ColumnLayout {
            Layout.maximumWidth: 300
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RightPanel {

                }
            }
        }
    }

    function splitImageToSprites() {
        spriteSheetGrid.initModel()
    }
}

