import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: settings_item.height

    property bool defaultValue
    property string fieldName: ""

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

        Rectangle {
            width: 100
            height: parent.height
            color: "transparent"

            CheckBox {
                id: checkbox
                anchors.centerIn: parent
                checked: defaultValue
                onCheckedChanged: {
                    changeValue(checked)
                }
            }

        }

    }

    Component.onCompleted: {
        checkbox.checked = defaultValue
        console.log("Created bool property with value: ", defaultValue)
    }

    function changeValue(value) {
        valueChanged(value)
    }

    signal valueChanged(bool value)
}

