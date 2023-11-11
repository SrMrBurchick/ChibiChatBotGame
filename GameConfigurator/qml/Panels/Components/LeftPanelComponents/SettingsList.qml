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
            fieldDescription: "Columns count"
            defaultValue: ActionsManager.tableSettingsColumns
            minValue: 1
            maxValue: 24
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
            fieldDescription: "Row count"
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

            fieldName: "Width:"
            fieldDescription: "Sprite width"
            defaultValue: ActionsManager.spriteSizeWidth
            onValueChanged:(value) => {
                ActionsManager.spriteSizeWidth = value
            }
            Component.onCompleted: {
                console.log("Configured width: ", ActionsManager.spriteSizeWidth)
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1

            fieldName: "Height:"
            fieldDescription: "Sprite height"
            defaultValue: ActionsManager.spriteSizeHeight
            onValueChanged:(value) => {
                ActionsManager.spriteSizeHeight = parseInt(value)
            }
        }

        PropertyRealDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Scale:"
            fieldDescription: "Sprite scale"
            defaultValue: ActionsManager.spriteScale
            onValueChanged:(value) => {
                ActionsManager.spriteScale = value
                console.log(ActionsManager.spriteScale)
            }
        }
    }
}
