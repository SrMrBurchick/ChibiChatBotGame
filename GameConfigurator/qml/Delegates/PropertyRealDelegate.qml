import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base

Rectangle {
    id: root
    height: settings_item.height

    property real defaultValue
    property string fieldName: ""
    property string fieldDescription: ""
    property real minValue: 0.01
    property real maxValue: 10.0

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
            id: spinbox
            from: parseInt(minValue * 100)
            to: parseInt(maxValue * 100)
            value: parseInt(defaultValue * 100)
            stepSize: 1
            Layout.margins: 10
            Layout.maximumWidth: 100
            Layout.minimumWidth: 100

            property int decimals: 2
            property real realValue: value / 100

            editable: true
            validator: DoubleValidator {
                bottom: Math.min(spinbox.from, spinbox.to)
                top:  Math.max(spinbox.from, spinbox.to)
            }

            textFromValue: function(value, locale) {
                return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 100
            }

            onValueModified: {
                changeValue(realValue)
            }

            background: Rectangle {
                color: Style.propertyDelegateBorderColor
            }

            contentItem: TextField {
                text: spinbox.textFromValue(spinbox.value, spinbox.locale)
                font: spinbox.font
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !spinbox.editable
                validator: spinbox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                placeholderText: fieldDescription
                color: Style.textColor
                onTextEdited: {
                    spinbox.value = spinbox.valueFromText(text, spinbox.locale)
                    changeValue(spinbox.value)
                }
                background: Rectangle {
                    color: Style.propertyDelegateBorderColor
                }
            }

        }
    }

    function changeValue(value) {
        valueChanged(value)
        console.log("Value modified: ", value)
    }

    signal valueChanged(real value)
}
