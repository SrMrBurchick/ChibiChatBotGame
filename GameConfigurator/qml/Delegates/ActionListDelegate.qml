import QtQuick
import QtQuick.Layouts
import Base
import Dialogs

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    border.color: index == delegateManager.currentIndex ? Style.actionListItemBorderSelectedColor : Style.actionListItemBorderColor
    border.width: 2
    width: parent.width - 5
    height: 50

    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

        MouseArea {
            Layout.fillWidth: true
            Layout.fillHeight: true

            BaseText {
                anchors.fill: parent
                id: actionName
                font.pixelSize : 24
                text: name
                anchors.verticalCenter: parent.verticalCenter
                // anchors.centerIn: panel
            }

            ChangeActionDialog {
                id: changeActionDialog
                actionName: actionName.text
                onChangeAction: {
                    changeElement(index, action);
                }

            }

            onClicked: {
                changeActionDialog.open()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    elementSelected(index, name)
                }
            }
        }

        Rectangle {
            width: 20
            height: 20
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.trashCanImage
                }
                onClicked: {
                    removeElement(index)
                }
            }
        }
    }

    Component.onCompleted: {

    }

    signal removeElement(int index)
    signal changeElement(int index, string action)
    signal elementSelected(int index, string action)
}
