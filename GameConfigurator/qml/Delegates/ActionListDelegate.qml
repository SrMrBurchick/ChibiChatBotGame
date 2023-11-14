import QtQuick
import QtQuick.Layouts
import Base
import Dialogs
import Panels

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    property bool isSelected: false
    property bool isDefaultAction: false
    readonly property var defaultActions: ["walk", "fall", "climb", "standby"]
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
                    actionName: actionName.text
                    onChangeAction: {
                        changeElement(index, action);
                    }

                }

                onClicked: {
                    elementSelected(index, name)
                }

                onDoubleClicked: {
                    if (!root.isDefaultAction) {
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
        ActionsManager.actionsListModel.onActionSelected.connect(function (action) {
            root.isSelected = index == ActionsManager.actionsListModel.getSelectedActionIndex()
        })

        if (defaultActions.includes(name)) {
            root.isDefaultAction = true
        }
    }

    signal removeElement(int index)
    signal changeElement(int index, string action)
    signal elementSelected(int index, string action)
}
