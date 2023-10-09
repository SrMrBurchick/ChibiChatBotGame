import QtQuick
import Panels

Rectangle {
    id: root
    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    color: "yellow"

    Text {
        text: "(" + row + " : " + column + ")"
    }

    Image {
        anchors.fill:
        source: ActionsManager.spriteSheetPath
        sourceClipRect: Qt.rect(
            parseInt(column) * ActionsManager.spriteSizeWidth,
            parseInt(row) * ActionsManager.spriteSizeHeight,
            ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(ActionsManager.spriteSheetPath)
        }
    }
}
