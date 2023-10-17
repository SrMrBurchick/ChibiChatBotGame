import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels

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

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Animation preview"
                }

                // TODO: Replace with AnimationSequance
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 200
                    height: 200
                    Image {
                        id: actionPreview
                        anchors.fill: parent
                        source: ActionsManager.spriteSheetPath
                        // sourceClipRect: Qt.rect(
                        //     parseInt(column) * ActionsManager.spriteSizeWidth,
                        //     parseInt(row) * ActionsManager.spriteSizeHeight,
                        //     ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
                    }

                }

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter

                    spacing: 4

                    Button {
                        text: "Play"
                        onClicked: {
                            timer.running = true
                        }
                    }

                    Button {
                        text: "Pause"
                        onClicked: {
                            timer.running = false
                        }
                    }

                }
            }
        }

        Button {
            Layout.fillWidth: true
            text: "Save"
            onClicked: saveActionsConfig()
        }

    }

    signal saveActionsConfig()

    function playAnimationPreview() {
        var nextSpriteData = ActionsManager.sequenceModel.getNextSprite()
        actionPreview.sourceClipRect = Qt.rect(
            parseInt(nextSpriteData.sprite_column) * ActionsManager.spriteSizeWidth,
            parseInt(nextSpriteData.sprite_row) * ActionsManager.spriteSizeHeight,
            ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight
        )
    }
}

