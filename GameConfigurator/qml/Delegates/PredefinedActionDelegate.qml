import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    width: parent.width - 5
    height: 50

    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

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

        TextField {
            Layout.margins: 10
            Layout.fillWidth: true
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            validator: RegularExpressionValidator{regularExpression: /^[0-9\./]+$/}
            background: Rectangle {
                color: Style.propertyDelegateBorderColor
            }
            text: chance
            font.pixelSize : 18
            color: Style.textColor
            onTextEdited: {
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
