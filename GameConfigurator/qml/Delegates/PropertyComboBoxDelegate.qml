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
    property var targetModel
    property var targetDelegate
    property int maxWidth: 100

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
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
        }

        ComboBox {
            id: comboBox
            Layout.margins: 10
            Layout.maximumWidth: maxWidth

            onActivated: {
                valueChanged(comboBox.currentText)
                setDisplayText(comboBox.popup.contentItem.currentItem.text)
            }

            Component.onCompleted: {
                if (targetModel != undefined) {
                    model = targetModel
                }
                if (targetDelegate != undefined) {
                    delegate = targetDelegate
                }

                calculateMaxWidth()
            }
        }
    }

    function calculateMaxWidth() {
        var childMaxWidth = 0

        // Iterate through the ComboBox's children to find the max width
        for (var i = 0; i < comboBox.popup.contentItem.children.length; i++) {
            var child = comboBox.popup.contentItem.children[i]
            if (child.width > maxWidth) {
                childMaxWidth = child.width
            }
        }

        if (maxWidth > root.maxWidth) {
            root.maxWidth = childMaxWidth + 20  // Add some padding for the dropdown arrow
        }
    }
    signal valueChanged(string value)

    function setDisplayText(value) {
        comboBox.displayText = value
    }
}

