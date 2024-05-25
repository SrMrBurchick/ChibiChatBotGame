pragma Singleton
import QtQuick
import QtQuick.Controls
import ActionsModels

QtObject {
    property string selectedAction
    property int actionSpriteOldIndex
    property int actionSpriteNewIndex
    property string spriteSheetPath: "qrc:qml/Images/sprite-sheet.png"
    property bool spriteSheetConfigured: false
    property bool isActionsConfigured: false

    // Table settings
    property int tableSettingsColumns
    property int tableSettingsRows

    // Sprite settings
    property int spriteSizeHeight
    property int spriteSizeWidth
    property real spriteScale: 1.0
    //
    // property ActionsListModel actionsListModel: ActionsListModel {}
    // property AnimationSequenceModel sequenceModel: AnimationSequenceModel {}
    // property SpriteSheetModel spriteSheetModel: SpriteSheetModel {}
}
