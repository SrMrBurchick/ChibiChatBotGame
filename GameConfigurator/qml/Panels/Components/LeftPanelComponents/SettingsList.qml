import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Panels
import GameActions
import Delegates
import Base
import Buttons

Column {
    id: root

    BaseText {
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Table settings"
        font.pixelSize: 24 // Set the font size
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        SettingsItemDelegate {
            fieldName: "Columns:"
            fieldDescription: "Columns count"
            defaultText: ActionsManager.tableSettingsColumns
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.tableSettingsColumns = parseInt(text)
                console.log(ActionsManager.tableSettingsColumns)
            }
        }

        SettingsItemDelegate {
            fieldName: "Rows:"
            fieldDescription: "Row count"
            defaultText: ActionsManager.tableSettingsRows
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.tableSettingsRows = parseInt(text)
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
        Layout.fillHeight: true

        SettingsItemDelegate {
            fieldName: "Width:"
            fieldDescription: "Sprite width"
            defaultText: ActionsManager.spriteSizeWidth
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.spriteSizeWidth = parseInt(text)
            }
        }

        SettingsItemDelegate {
            fieldName: "Height:"
            fieldDescription: "Sprite height"
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            defaultText: ActionsManager.spriteSizeHeight
            onValueChanged:(text) => {
                ActionsManager.spriteSizeHeight = parseInt(text)
            }
        }
    }
}
