import QtQuick
import QtQuick.Layouts
import Base
import Dialogs
import Panels
import Managers

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    property bool isSelected: false
    border.color: isSelected ? Style.actionListItemBorderSelectedColor : Style.actionListItemBorderColor
    border.width: 2
    width: parent.width - 5
    height: 50

    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

        BaseText {
            anchors.fill: parent
            id: actionName
            font.pixelSize : 24
            text: name
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent

                ChangeActionDialog {
                    id: changeActionDialog
                    actionName: name
                    onChangeAction: {
                        changeElement(index, action);
                    }
                }

                onClicked: {
                    elementSelected(index, name)
                }

                onDoubleClicked: {
                    if (!ActionsManager.isDefaultAction(index)) {
                        changeActionDialog.open()
                    }
                }
            }
        }

        Rectangle {
            width: 20
            height: 20
            color: "transparent"
            anchors.right: parent.right

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
        if (ActionsManager != undefined)
        {
            var selectedAction = ActionsManager.getSelectedAction()
            if (selectedAction != undefined)
            {
                    root.isSelected = name == selectedAction.name
            }

            ActionsManager.onActionSelected.connect(function (selectedAction) {
                if (selectedAction != undefined) {
                    root.isSelected = name == selectedAction.getName()
                }
            })
        }
    }

    signal removeElement(int index)
    signal changeElement(int index, string action)
    signal elementSelected(int index, string action)
}
