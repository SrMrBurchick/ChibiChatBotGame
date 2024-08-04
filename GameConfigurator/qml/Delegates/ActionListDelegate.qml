import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Panels
import Managers

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    property bool isSelected: false
    property string actionIcon
    property string actionTooltip
    border.color: isSelected ? Style.actionListItemBorderSelectedColor : Style.actionListItemBorderColor
    border.width: 2
    width: parent.width - 5
    height: 50

    Rectangle {
        id: actionType
        width: 40
        height: 40
        color: "transparent"
        anchors.left: root.left
        anchors.top: root.top
        anchors.margins: 5

        Image {
            anchors.fill: parent
            source: actionIcon
        }

        ToolTip.delay: 1000
        ToolTip.timeout: 5000
        ToolTip.visible: hovered
        ToolTip.text: actionTooltip

        MouseArea {
            anchors.fill: parent
            onClicked: {
                elementSelected(index, name)
            }
        }

    }

    Rectangle {
        id: deleteItem
        width: 30
        height: 30
        color: "transparent"
        anchors.right: root.right
        anchors.top: root.top
        anchors.margins: 10

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

    MouseArea {
        anchors.left: actionType.right
        anchors.right: deleteItem.left
        anchors.top: root.top
        anchors.bottom: root.bottom

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


    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

        BaseText {
            id: actionName
            font.pixelSize : 24
            text: name
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Component.onCompleted: {
        if (ActionsManager != undefined)
        {
            var selectedAction = ActionsManager.getSelectedAction()
            if (selectedAction != undefined) {
                root.isSelected = name == selectedAction.name
            }

            if (ActionsManager.isGameDefaultAction(ActionsManager.getActionByName(name))) {
                actionIcon = Style.templateIcon
                actionTooltip = GlobalConfig.gameDefaultActionTooltip
            } else if (ActionsManager.isTwitchDefaultAction(ActionsManager.getActionByName(name))) {
                actionIcon = Style.twitchIcon
                actionTooltip = GlobalConfig.twitchDefaultActionTooltip
            } else {
                actionIcon = Style.gearIcon
                actionTooltip = GlobalConfig.customActionTooltip
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
