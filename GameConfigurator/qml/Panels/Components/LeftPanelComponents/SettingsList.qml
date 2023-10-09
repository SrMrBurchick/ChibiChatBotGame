import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Panels
import GameActions

ColumnLayout {
    anchors.fill: parent
    anchors.margins: 10

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Table settings"
        font.pixelSize: 24 // Set the font size
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        RowLayout {
            Text {
                text: "columns"
            }
            Rectangle {
                Layout.fillWidth: true
            }
            TextEdit {
                text: "0"
                onEditingFinished: {
                    ActionsManager.tableSettingsColumns = parseInt(text)
                }
            }
        }

        RowLayout {
            Text {
                text: "rows"
            }
            Rectangle {
                Layout.fillWidth: true
            }
            TextEdit {
                text: "0"
                onEditingFinished: {
                    ActionsManager.tableSettingsRows = parseInt(text)
                }

            }
        }
    }

    Text {
        text: "Sprite settings"
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 24 // Set the font size
    }

    ColumnLayout {

        RowLayout {
            Text {
                text: "width"
            }
            Rectangle {
                Layout.fillWidth: true
            }
            TextEdit {
                text: "0"
                onEditingFinished: {
                    ActionsManager.spriteSizeWidth = parseInt(text)
                }
            }
        }

        RowLayout {
            Text {
                text: "height"
            }
            Rectangle {
                Layout.fillWidth: true
            }
            TextEdit {
                text: "0"

                onEditingFinished: {
                    ActionsManager.spriteSizeHeight = parseInt(text)
                }
            }
        }
    }

    Button {
        Layout.fillWidth: true
        text: "Split to sprites"
        onClicked: {
            ActionsManager.spriteSheetConfigured = true
            splitImageToSprites()
        }
    }

    signal splitImageToSprites()
}
