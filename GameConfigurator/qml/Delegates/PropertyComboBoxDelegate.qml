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
    required property var targetModel
    required property variant targetDelegate

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

        ComboBox {
            id: comboBox
            Layout.margins: 10
            Layout.fillWidth: true
            model: targetModel
            delegate: targetDelegate
            onActivated: {
                valueChanged(comboBox.currentText)
                comboBox.displayText = comboBox.popup.contentItem.currentItem.text
            }
        }
    }

    signal valueChanged(string value)
}

