pragma Singleton
import QtQuick
import QtQuick.Controls
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

    // System settings
    property string twitchChannel
    property string chatBotURL: "localhost"
    property int chatBotPort: 6565

    property AnimationSequenceModel sequenceModel: AnimationSequenceModel {}
    property SpriteSheetModel spriteSheetModel: SpriteSheetModel {}
}
