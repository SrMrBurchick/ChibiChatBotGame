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
    property real spriteScale: 1.0

    // System settings
    property string twitchChannel
    property string chatBotURL: "localhost"
    property int chatBotPort: 6565
    property int screenWidth: 1920
    property int screenHeight: 1200

    property AnimationSequenceModel sequenceModel: AnimationSequenceModel {}
    property SpriteSheetModel spriteSheetModel: SpriteSheetModel {}
}
