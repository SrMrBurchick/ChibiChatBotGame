import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Delegates

Rectangle {
    id: root
    width: parent.width
    height: panel.height

    color: Style.propertyDelegateBGColor
    border.color: Style.propertyDelegateBorderColor
    border.width: 2

    RowLayout {
        id: panel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        BaseText {
            id: actionName
            font.pixelSize : 24
            text: name
            anchors.verticalCenter: parent.verticalCenter
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
            from: 1
            to: 100
            value: chance
            stepSize: 1
            editable: true
            contentItem: TextField {
                text: spinbox.value
                font: spinbox.font
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !spinbox.editable
                validator: spinbox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                color: Style.textColor
                onTextChanged: {
                    spinbox.value = parseInt(text);
                    changeChance(index, spinbox.value)
                }
                background: Rectangle {
                    color: Style.propertyDelegateBorderColor
                }

            }

            background: Rectangle {
                color: Style.propertyDelegateBorderColor
            }

            onValueModified: {
                changeChance(index, spinbox.value)
            }
        }

        Rectangle {
            width: 20
            height: 20
            color: "transparent"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.trashCanImage
                }
                onClicked: {
                    removeElement(index);
                }
            }
        }
    }

    signal removeElement(int index)
    signal changeChance(int index, real chance)
}
