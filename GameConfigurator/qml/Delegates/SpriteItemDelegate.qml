import QtQuick
import Panels
import Base

Rectangle {
    id: root
    color: Style.imagePreviewBGColor
    property bool isSelected: false

    BaseText {
        text: "(" + sprite_row + " : " + sprite_column + ")"
    }

    Image {
        id: image
        anchors.fill: parent
        // source: ActionsManager.spriteSheetPath
        // sourceClipRect: Qt.rect(
        //     parseInt(sprite_column) * ActionsManager.spriteSizeWidth,
        //     parseInt(sprite_row) * ActionsManager.spriteSizeHeight,
        //     ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
    }

    Rectangle {
        anchors.fill: parent
        color: root.isSelected ? Style.spriteItemSelectedBGColor : "transparent"
        opacity: 0.6

        BaseText {
            id: actionsCount
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            visible: isSelected
        }

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                root.isSelected = true
                toggleSelected(index, sprite_column, sprite_row)
                // actionsCount.text = ActionsManager.spriteSheetModel.getActionsCount(index, ActionsManager.actionsListModel.getSelectedAction())
            }
        }
    }

    Component.onCompleted: {
        // root.isSelected = ActionsManager.spriteSheetModel.containsAction(index, ActionsManager.actionsListModel.getSelectedAction())
        // ActionsManager.actionsListModel.onActionSelected.connect(function (action) {
        //     root.isSelected = ActionsManager.spriteSheetModel.containsAction(index, action)
        //     actionsCount.text = ActionsManager.spriteSheetModel.getActionsCount(index, action)
        // })
        //
        // actionsCount.text = ActionsManager.spriteSheetModel.getActionsCount(index, ActionsManager.actionsListModel.getSelectedAction())
    }

    signal toggleSelected(int index, int sprite_column, int sprite_row)

    function sequenceUpdated() {
        // root.isSelected = ActionsManager.spriteSheetModel.containsAction(index, ActionsManager.actionsListModel.getSelectedAction())
        // actionsCount.text = ActionsManager.spriteSheetModel.getActionsCount(index, ActionsManager.actionsListModel.getSelectedAction())
    }
}
