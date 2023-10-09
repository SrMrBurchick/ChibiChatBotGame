import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import GameActions
import Panels

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: panel
        anchors.fill: root

        Text {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Sprite sheet"
        }

        Rectangle {
            id: spritesGrid
            anchors.top: title.bottom
            anchors.bottom: panel.bottom

            width: panel.width

            border.color: "black"
            border.width: 2
            color: "white"

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
                        height: gridView.cellHeight
                        width: gridView.cellWidth
                    }
                }
            }
        }
    }

    function initModel() {
        spriteSheetModel.initModel(ActionsManager.tableSettingsColumns, ActionsManager.tableSettingsRows)
    }
}
