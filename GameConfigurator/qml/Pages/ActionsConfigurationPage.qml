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
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "green"

                Text {
                    anchors.centerIn: parent
                    text: "Middle panel"
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

