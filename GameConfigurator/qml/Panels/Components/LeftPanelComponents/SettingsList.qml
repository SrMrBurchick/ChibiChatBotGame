import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Panels
import GameActions
import Delegates
import Base
import Buttons

ColumnLayout {
    id: root

    BaseText {
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Table settings"
        font.pixelSize: 24 // Set the font size
    }

    ColumnLayout {
        Layout.fillWidth: true

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Columns:"
            minValue: 1
            maxValue: 24
            defaultValue: ActionsManager.tableSettingsColumns
            onValueChanged:(value) => {
                ActionsManager.tableSettingsColumns = value
                console.log(ActionsManager.tableSettingsColumns)
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 24

            fieldName: "Rows:"
            defaultValue: ActionsManager.tableSettingsRows
            onValueChanged:(value) => {
                ActionsManager.tableSettingsRows = value
                console.log(ActionsManager.tableSettingsRows)
            }
        }
    }

    BaseText {
        text: "Sprite settings"
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 24 // Set the font size
    }

    ColumnLayout {
        spacing: 5
        Layout.fillWidth: true

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 1000

            fieldName: "Width:"
            defaultValue: ActionsManager.spriteSizeWidth
            onValueChanged:(value) => {
                ActionsManager.spriteSizeWidth = value
                console.log(ActionsManager.spriteSizeWidth)
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 1000

            fieldName: "Height:"
            defaultValue: ActionsManager.spriteSizeHeight
            onValueChanged:(value) => {
                ActionsManager.spriteSizeHeight = value
                console.log(ActionsManager.spriteSizeHeight)
            }
        }

        PropertyRealDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Scale:"
            defaultValue: ActionsManager.spriteScale
            onValueChanged:(value) => {
                ActionsManager.spriteScale = value
                console.log(ActionsManager.spriteScale)
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
