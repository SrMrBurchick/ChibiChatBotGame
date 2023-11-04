import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: settings_item.height
    width: parent.width

    property string defaultText
    property string fieldName: ""
    property string fieldDescription: ""
    property RegularExpressionValidator typeValidator: RegularExpressionValidator{}

    color: Style.settingsItemBGColor
    border.color: Style.settingsItemBorderColor
    border.width: 2

    RowLayout {
        id: settings_item
        width: root.width
        anchors.centerIn: root

        BaseText {
            text: fieldName
            Layout.margins: 10
            font.pixelSize : 24
        }
        Rectangle {
            opacity: 0
            Layout.fillWidth: true
        }
        TextField {
            Layout.margins: 10
            Layout.fillWidth: true
            placeholderText: fieldDescription
            horizontalAlignment: TextInput.AlignHCenter
            validator: typeValidator
            background: Rectangle {
                color: Style.settingsItemBorderColor
            }
            text: defaultText
            font.pixelSize : 18
            color: Style.textColor
            onTextEdited: {
                valueChanged(text)
            }
        }
    }

    signal valueChanged(string text)
}

