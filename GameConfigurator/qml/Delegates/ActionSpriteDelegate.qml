import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import Base
import ConfigComponent
import Managers

Rectangle {
    id: root
    property string imageSource
    property int frameSize

    color: Style.imagePreviewBGColor

    Drag.active: dragArea.drag.active

    BaseText {
        text: "(" + sprite_row + " : " + sprite_column + ")"
        font.pixelSize : 24
    }

    Image {
        id: image
        anchors.fill: parent
        source: Config.systemConfig.imagePath
        sourceClipRect: Qt.rect(
            parseInt(sprite_column) * Config.spriteSize.width,
            parseInt(sprite_row) * Config.spriteSize.height,
            Config.spriteSize.width, Config.spriteSize.height
        )

        mirror: inverted
    }

    MouseArea {
        id: dragArea
        anchors.fill: root

        drag.axis: Drag.XAxis
        drag.target: root

        onReleased: {
            var selectedAction = ActionsManager.getSelectedAction()
            if (selectedAction != undefined)
            {
                selectedAction.placeSpriteAt(index, selectedAction.newIndex)
                selectedAction.newIndex = -1
            }
        }

        onDoubleClicked: {
            var selectedAction = ActionsManager.getSelectedAction()
            if (selectedAction != undefined)
            {
                selectedAction.toggleInverted(index)
            }
        }
    }

    DropArea {
        anchors.fill: root
        onEntered:(drag) => {
            var selectedAction = ActionsManager.getSelectedAction()
            if (selectedAction != undefined)
            {
                selectedAction.newIndex = dragArea.DelegateModel.itemsIndex
            }
        }
    }
    Image {
        id: removeButton
        height: root.height / 10
        width: height
        anchors.top: image.top
        anchors.right: image.right
        source: Style.trashCanImage


        MouseArea {
            anchors.fill: parent

            onDoubleClicked: {
                var selectedAction = ActionsManager.getSelectedAction()
                if (selectedAction != undefined)
                {
                    selectedAction.removeSprite(index)
                }
            }
        }
    }
}
