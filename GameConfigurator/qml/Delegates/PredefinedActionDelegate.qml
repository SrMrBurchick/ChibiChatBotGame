import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Delegates
import Buttons

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

            onValueModified: {
                changeChance(index, spinbox.value)
            }
        }

        RemoveButton {
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                removeElement(index);
            }
        }
    }

    signal removeElement(int index)
    signal changeChance(int index, real chance)
}
