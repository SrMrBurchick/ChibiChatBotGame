import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels

Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view

    signal goBack()
    signal save()
    signal loadSpriteSheet()

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
                LeftPanel {
                    onGoBack: {
                        rootStack.pop()
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

                color: "blue"

                Text {
                    anchors.centerIn: parent
                    text: "Right"
                }

            }
        }
    }
}

