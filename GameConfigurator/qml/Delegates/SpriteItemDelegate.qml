import QtQuick
import Panels

Rectangle {
    id: root
    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    color: "yellow"

    Text {
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
        anchors.fill: parent
        // onClicked: {
        //     console.log(ActionsManager.spriteSheetPath)
        // }
        onDoubleClicked: {
            toggleSelected(index, sprite_column, sprite_row)
        }
    }

    signal toggleSelected(int index, int sprite_column, int sprite_row)
}
