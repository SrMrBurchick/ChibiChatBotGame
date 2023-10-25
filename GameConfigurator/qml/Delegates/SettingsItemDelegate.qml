import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: 40

    Layout.margins: 10
    width: parent.width - 20

    property string defaultText
    property string fieldName: ""
    property string fieldDescription: ""
    property RegularExpressionValidator typeValidator: RegularExpressionValidator{}

    color: Style.settingsItemBGColor
    border.color: Style.settingsItemBorderColor
    border.width: 2

    RowLayout {
        anchors.fill: root
        anchors.margins: 10

        BaseText {
            text: fieldName
        }
        Rectangle {
            opacity: 0
            Layout.fillWidth: true
        }
        TextField {
            placeholderText: fieldDescription
            horizontalAlignment: TextInput.AlignHCenter
            validator: typeValidator
            text: defaultText
            onTextEdited: {
                valueChanged(text)
            }
        }
    }

    signal valueChanged(string text)
}

