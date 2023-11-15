import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: settings_item.height

    property int defaultValue
    property string fieldName: ""
    property string fieldDescription: ""
    property int minValue: 0
    property int maxValue

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

        SpinBox {
            Layout.margins: 10
            Layout.maximumWidth: 100
            Layout.minimumWidth: 100
            id: spinbox
            from: minValue
            to: maxValue
            value: defaultValue
            editable: true
            stepSize: 1

            onValueModified: {
                changeValue(spinbox.value)
            }
        }
    }

    Component.onCompleted: {
        spinbox.value = defaultValue
        console.log("Created int property with value: ", defaultValue)
    }

    function changeValue(value) {
        console.log("Value modified: ", value)
        valueChanged(value)
    }

    signal valueChanged(int value)
}

