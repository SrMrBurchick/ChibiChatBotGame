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
            defaultValue: Config.tableSize.columns
            onValueChanged:(value) => {
                Config.tableSize.columns = value
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 24

            fieldName: "Rows:"
            defaultValue: Config.tableSize.rows
            onValueChanged:(value) => {
                Config.tableSize.rows = value
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
            defaultValue: Config.spriteSize.width
            onValueChanged:(value) => {
                Config.spriteSize.width = value
            }
        }

        PropertyIntDelegate {
            Layout.fillWidth: true
            Layout.margins: 10
            minValue: 1
            maxValue: 1000

            fieldName: "Height:"
            defaultValue: Config.spriteSize.height
            onValueChanged:(value) => {
                Config.spriteSize.height = value
            }
        }

        PropertyRealDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Scale:"
            defaultValue: Config.systemConfig.gameSettings.spriteScale
            onValueChanged:(value) => {
                Config.systemConfig.gameSettings.spriteScale = value
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
