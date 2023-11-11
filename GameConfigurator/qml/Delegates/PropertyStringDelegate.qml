import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: settings_item.height

    property string defaultValue
    property string fieldName: ""
    property string fieldDescription: ""

    color: Style.propertyDelegateBGColor
    border.color: Style.propertyDelegateBorderColor
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
                color: Style.propertyDelegateBorderColor
            }
            text: defaultValue
            font.pixelSize : 18
            color: Style.textColor
            onTextEdited: {
                valueChanged(text)
            }
        }
    }

    signal valueChanged(string value)
}

