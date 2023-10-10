pragma Singleton
import QtQuick
import GameActions

QtObject {
    property string selectedAction
    property int actionSpriteOldIndex
    property int actionSpriteNewIndex
    property string spriteSheetPath
    property bool spriteSheetConfigured

    // Table settings
    property int tableSettingsColumns
    property int tableSettingsRows

    // Sprite settings
    property int spriteSizeHeight
    property int spriteSizeWidth

    property AnimationSequenceModel sequenceModel: AnimationSequenceModel {}
    property SpriteSheetModel spriteSheetModel: SpriteSheetModel {}
}
