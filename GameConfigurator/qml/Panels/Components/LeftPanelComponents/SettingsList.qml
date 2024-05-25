import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Panels
import ActionsModels
import Delegates
import Base
import Buttons
import ConfigComponent

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
            defaultValue: Config.getTableColumns()
            onValueChanged:(value) => {
                Config.saveTableColumns(value)
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 24

            fieldName: "Rows:"
            defaultValue: Config.getTableRows()
            onValueChanged:(value) => {
                Config.saveTableRows(value)
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
            defaultValue: Config.getSpriteWidth()
            onValueChanged:(value) => {
                GlobalCongi.saveSpriteWidth(value)
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 1000

            fieldName: "Height:"
            defaultValue: Config.getSpriteHeight()
            onValueChanged:(value) => {
                Config.saveSpriteHeight(value)
            }
        }

        PropertyRealDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Scale:"
            defaultValue: Config.getSpriteScale()
            onValueChanged:(value) => {
                Config.saveSpriteScale(value)
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
