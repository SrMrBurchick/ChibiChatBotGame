import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons

Item {
    id: root
    anchors.fill: parent

    // Main layout
    RowLayout {
        anchors.fill: parent

        // Left panel
        ColumnLayout {
            Layout.maximumWidth: 400
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "red"

                Text {
                    anchors.centerIn: parent
                    text: "Left panel"
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
            Layout.maximumWidth: 400
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

    signal goBack()
    signal save()
    signal loadSpriteSheet()
}

