import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import GameActions
import Panels
import Base

Item {
    id: root
    anchors.fill: parent

    BasePanel {
        id: panel
        anchors.fill: root

        BaseText {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Sprite sheet"
            font.pixelSize : 32
        }

        BasePanel {
            id: spritesGrid
            anchors.top: title.bottom
            anchors.bottom: panel.bottom

            width: panel.width

            ScrollView {
                clip: true
                anchors.fill: spritesGrid

                GridView {
                    id: gridView
                    anchors.fill: parent
                    flow: GridView.LeftToRight
                    cellWidth: (spritesGrid.width / ActionsManager.tableSettingsColumns)
                    cellHeight:(spritesGrid.width / ActionsManager.tableSettingsColumns)
                    model: SpriteSheetModel {
                        id: spriteSheetModel
                    }

                    delegate: SpriteItemDelegate {
                        id: sprite
                        height: gridView.cellHeight
                        width: gridView.cellWidth
                        onToggleSelected:(index, column, row) => {
                            if (ActionsManager.sequenceModel != undefined) {
                                ActionsManager.sequenceModel.addNewAction(column, row)
                            }
                        }
                    }
                }
            }
        }
    }

    function initModel() {
        spriteSheetModel.initModel(ActionsManager.tableSettingsColumns, ActionsManager.tableSettingsRows)
    }
    function clearModel() {
        spriteSheetModel.clearModel()
    }

    Component.onCompleted: {
        ActionsManager.spriteSheetModel = spriteSheetModel
    }
}
