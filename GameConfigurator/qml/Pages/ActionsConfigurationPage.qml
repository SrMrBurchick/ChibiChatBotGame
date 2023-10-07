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
            // image.source = selectedFile
            spriteSheetPath = selectedFile
            console.log(spriteSheetPath)

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
                id: sprite_grid
                Layout.fillWidth: true
                Layout.fillHeight: true

                SpriteSheetGrid {
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 200
                border.color: "black"
                border.width: 2
                color: "white"

                // Text {
                //     text: middle.height + "x" + middle.width
                // }
                // height: parent.height / 3

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
}

