import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import Base

Rectangle {
    id: root
    property string imageSource
    property int frameSize

    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    Drag.active: dragArea.drag.active

    BaseText {
        text: "(" + sprite_column + " : " + sprite_row + ")"
    }

    Image {
        id: image
        anchors.fill: parent
        source: ActionsManager.spriteSheetPath
        sourceClipRect: Qt.rect(
            parseInt(sprite_column) * ActionsManager.spriteSizeWidth,
            parseInt(sprite_row) * ActionsManager.spriteSizeHeight,
            ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
    }

    MouseArea {
        id: dragArea
        anchors.fill: root

        drag.axis: Drag.XAxis
        drag.target: root

        onReleased: {
            placeItemAtPosition(ActionsManager.actionSpriteOldIndex, ActionsManager.actionSpriteNewIndex)

            ActionsManager.actionSpriteOldIndex = -1
            ActionsManager.actionSpriteNewIndex = -1

        }
    }

    DropArea {
        anchors.fill: root
        onEntered:(drag) => {
            ActionsManager.actionSpriteOldIndex = drag.source.DelegateModel.itemsIndex
            ActionsManager.actionSpriteNewIndex = dragArea.DelegateModel.itemsIndex
        }
    }
    Rectangle {
        id: removeButton
        height: root.height / 5
        width: height
        anchors.top: image.top
        anchors.right: image.right

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                removeItem(index)
            }
        }
    }

    signal toggleInverted(int index)
    signal placeItemAtPosition(int oldIndex, int newIndex)
    signal removeItem(int index)
}
