import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ConfigComponent
import Base
import Managers

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
                        source: Config.systemConfig.imagePath
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
        var selectedAction = ActionsManager.getSelectedAction()
        if (selectedAction != undefined)
        {
            var nextSpriteData = selectedAction.getNextSprite()
            actionPreview.sourceClipRect = Qt.rect(
                parseInt(nextSpriteData.sprite_column) * Config.spriteSize.width,
                parseInt(nextSpriteData.sprite_row) * Config.spriteSize.height,
                Config.spriteSize.width, Config.spriteSize.height
            )
            actionPreview.mirror = nextSpriteData.isInverted;
        }
    }
}
