import QtQuick
import QtQuick.Layouts
import Base
import Dialogs

Rectangle {
    id: root
    property int actionIndex: 0

    border.color: actionIndex == delegateManager.currentIndex ? "lightblue" : "black"
    border.width: 2

    RowLayout {
        anchors.margins: 4
        anchors.fill: root

        MouseArea {
            Layout.fillWidth: true
            Layout.fillHeight: true

            BaseText {
                anchors.fill: parent
                id: actionName
                text: name
            }

            ChangeActionDialog {
                id: changeActionDialog
                actionName: actionName.text
                onChangeAction: {
                    changeElement(actionIndex, action);
                }

            }

            onClicked: {
                changeActionDialog.open()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    elementSelected(actionIndex, name)
                }
            }
        }

        Rectangle {
            width: 20
            height: 20

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.trashCanImage
                }
                onClicked: {
                    removeElement(actionIndex);
                }
            }
        }
    }

    signal removeElement(int index)
    signal changeElement(int index, string action)
    signal elementSelected(int index, string action)
}
