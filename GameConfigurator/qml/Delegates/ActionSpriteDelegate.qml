import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels

Rectangle {
    id: root
    property string imageSource
    property int frameSize

    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    Drag.active: dragArea.drag.active

    Text {
        text: "(" + column + " : " + row + ")"
    }

    Image {
        id: image
        anchors.fill: parent
        source: ActionsManager.spriteSheetPath
        sourceClipRect: Qt.rect(
            parseInt(column) * ActionsManager.spriteSizeWidth,
            parseInt(row) * ActionsManager.spriteSizeHeight,
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

    signal toggleInverted(int index)
    signal placeItemAtPosition(int oldIndex, int newIndex)
}
