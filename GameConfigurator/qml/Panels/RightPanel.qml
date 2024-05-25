import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ConfigComponent
import Base

Item {
    id: root
    anchors.fill: parent
    anchors.leftMargin: 6

    Timer {
        id: timer
        interval: 100
        running: false
        repeat: true

        onTriggered: {
            root.playAnimationPreview()
        }
    }

    ColumnLayout {
        id: panel
        anchors.fill: root

        BasePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                BaseText {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Animation preview"
                    font.pixelSize : 28
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 200
                    height: 200
                    color: Style.imagePreviewBGColor
                    Image {
                        id: actionPreview
                        anchors.fill: parent
                        // source: ActionsManager.spriteSheetPath
                    }

                }

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.fillWidth: true

                    spacing: 4

                    BaseButton {
                        Layout.fillWidth: true
                        text: timer.running ? "Pause" : "Play"
                        onClicked: {
                            timer.running = !timer.running
                        }
                    }
                }
            }
        }
    }

    function playAnimationPreview() {
        // var nextSpriteData = ActionsManager.sequenceModel.getNextSprite()
        // actionPreview.sourceClipRect = Qt.rect(
        //     parseInt(nextSpriteData.sprite_column) * ActionsManager.spriteSizeWidth,
        //     parseInt(nextSpriteData.sprite_row) * ActionsManager.spriteSizeHeight,
        //     ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight
        // )
        actionPreview.mirror = nextSpriteData.isInverted;
    }
}

