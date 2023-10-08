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

    MouseArea {
        id: dragArea
        anchors.fill: root

        drag.axis: Drag.XAxis
        drag.target: root

        onReleased: {
            placeItemAtPosition(ActionsManager.actionSpriteOldIndex, ActionsManager.actionSpriteNewIndex)
        }
    }


    DropArea {
        anchors.fill: root
        onEntered:(drag) => {
            ActionsManager.actionSpriteOldIndex = drag.source.DelegateModel.itemsIndex
            ActionsManager.actionSpriteNewIndex = dragArea.DelegateModel.itemsIndex
        }
    }

    signal placeItemAtPosition(int oldIndex, int newIndex)
}
