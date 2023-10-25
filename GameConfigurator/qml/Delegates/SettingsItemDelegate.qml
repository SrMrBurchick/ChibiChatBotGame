import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    width: parent.width
    height: 40

    anchors.margins: 10

    property string defaultText
    property string fieldName: ""
    property string fieldDescription: ""
    property RegularExpressionValidator typeValidator: RegularExpressionValidator{}

    RowLayout {
        anchors.fill: root

        BaseText {
            text: fieldName
        }
        Rectangle {
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

