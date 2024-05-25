import QtQuick
import Panels
import Base
import ConfigComponent
import ActionsManagerComponent

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
        source: Config.getSpriteSheetPath()
        sourceClipRect: Qt.rect(
            parseInt(sprite_column) * Config.getSpriteWidth(),
            parseInt(sprite_row) * Config.getSpriteHeight(),
            Config.getSpriteWidth(), Config.getSpriteHeight())
    }

    Rectangle {
        anchors.fill: parent
        color: root.isSelected ? Style.spriteItemSelectedBGColor : "transparent"
        opacity: 0.6

        BaseText {
            id: actionsCount
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            visible: root.isSelected
        }

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                root.isSelected = true
                toggleSelected(index, sprite_column, sprite_row)

                var selectedAction = ActionsManager.getSelectedAction()
                if (selectedAction != undefined)
                {
                    root.isSelected = selectedAction.hasSprite(sprite_column, sprite_row)
                    actionsCount.text = selectedAction.getSpriteCounts(sprite_column, sprite_row)
                }
            }
        }
    }

    Component.onCompleted: {
        if (ActionsManager != undefined)
        {
            ActionsManager.onActionSelected.connect(function (selectedAction) {
                if (selectedAction != undefined) {
                    root.isSelected = selectedAction.hasSprite(sprite_column, sprite_row)
                    actionsCount.text = selectedAction.getSpriteCounts(sprite_column, sprite_row)
                }
            })
        }
    }

    signal toggleSelected(int index, int sprite_column, int sprite_row)

    function sequenceUpdated() {
        // root.isSelected = ActionsManager.spriteSheetModel.containsAction(index, ActionsManager.actionsListModel.getSelectedAction())
        // actionsCount.text = ActionsManager.spriteSheetModel.getActionsCount(index, ActionsManager.actionsListModel.getSelectedAction())
    }
}
