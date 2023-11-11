import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Base

Rectangle {
    id: root
    height: settings_item.height
    width: parent.width

    property color defaultValue: "red"
    property string fieldName: ""
    property string fieldDescription: ""
    property RegularExpressionValidator typeValidator: RegularExpressionValidator{}

    color: Style.propertyDelegateBGColor
    border.color: Style.propertyDelegateBorderColor
    border.width: 2

    RowLayout {
        id: settings_item
        width: root.width
        anchors.centerIn: root

        BaseText {
            id: description
            text: fieldName
            Layout.margins: 10
            font.pixelSize : 24
        }

        Rectangle {
            opacity: 0
            Layout.fillWidth: true
        }

        Rectangle {
            id: textColor
            Layout.margins: 10
            height: description.height
            width: 100
            radius: 10
            color: defaultValue
            MouseArea {
                anchors.fill: parent
                ColorDialog {
                    id: colorDialog
                    title: "Please choose a color"
                    selectedColor: textColor.color
                    onAccepted: {
                        textColor.color = colorDialog.selectedColor
                        valueChanged(textColor.color)
                    }
                }

                onClicked: {
                    colorDialog.open()
                }
            }
        }
    }

    signal valueChanged(color value)
}

