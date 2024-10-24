import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import ActionsModels
import Panels
import Base
import ConfigComponent
import Managers

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
                    model: SpriteSheetModel {}

                    delegate: SpriteItemDelegate {
                        id: sprite
                        height: gridView.cellHeight
                        width: gridView.cellWidth
                        onToggleSelected:(index, column, row) => {
                            var action = ActionsManager.getSelectedAction()
                            if (action != undefined)
                            {
                                action.addNewSprite(column, row)
                            }
                        }
                        Component.onCompleted: {
                            root.onSequenceUpdated.connect(function(){
                                sprite.sequenceUpdated()
                            })
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        initModel()
    }

    function initModel() {
        gridView.cellWidth = (panel.width / Config.tableSize.columns)
        gridView.cellHeight = (panel.width / Config.tableSize.columns)

        gridView.model.initModel(
            Config.tableSize.columns,
            Config.tableSize.rows
        )
    }

    function clearModel() {
        gridView.model.clearModel()
    }

    signal sequenceUpdated()
}
