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
    property bool isEditable: true

    color: Style.propertyDelegateBGColor
    border.color: Style.propertyDelegateBorderColor
    border.width: 2

    Component {
        id: editableText
        TextField {
            Layout.margins: 10
            Layout.fillWidth: true
            placeholderText: fieldDescription
            horizontalAlignment: TextInput.AlignHCenter
            validator: typeValidator
            background: Rectangle {
                color: Style.propertyDelegateBorderColor
            }
            text: defaultText
            font.pixelSize : 18
            color: Style.textColor
            onTextEdited: {
                valueChanged(text)
            }
        }
    }

    Component {
        id: nonEditableText
        Text {
            Layout.margins: 10
            Layout.fillWidth: true
            horizontalAlignment: TextInput.AlignHCenter
            text: defaultText
            font.pixelSize : 18
            color: Style.textColor
        }
    }

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

        Loader {
            id: contentLoader
            visible: true
            Layout.fillWidth: true
            Layout.margins: 10

            sourceComponent: root.isEditable ? editableText : nonEditableText
        }
    }


    signal valueChanged(string text)
}

