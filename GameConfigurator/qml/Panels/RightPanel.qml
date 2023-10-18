import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ConfigComponent

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

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 200
                    height: 200
                    Image {
                        id: actionPreview
                        anchors.fill: parent
                        source: ActionsManager.spriteSheetPath
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

    function saveActionsConfig() {
        Config.saveSpriteSheetPath(ActionsManager.spriteSheetPath)
        Config.saveSpriteSettings(ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
        Config.saveTableSettings(ActionsManager.tableSettingsColumns, ActionsManager.tableSettingsRows)
        Config.saveActions(ActionsManager.sequenceModel)
        Config.saveConfig()
    }

    function playAnimationPreview() {
        var nextSpriteData = ActionsManager.sequenceModel.getNextSprite()
        actionPreview.sourceClipRect = Qt.rect(
            parseInt(nextSpriteData.sprite_column) * ActionsManager.spriteSizeWidth,
            parseInt(nextSpriteData.sprite_row) * ActionsManager.spriteSizeHeight,
            ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight
        )
    }
}

