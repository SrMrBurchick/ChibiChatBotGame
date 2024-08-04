pragma Singleton
import QtQuick

QtObject {
    readonly property string gameDefaultActionTooltip: "This is a default action for game!\nIt cannot use any settings except twitch.\nGame default actions uses by game for movement!"
    readonly property string twitchDefaultActionTooltip: "This is a default action for twitch.\nThat action cannot binds to any other twitch actions.\nBut you can provide text, and custom animation for it!"
    readonly property string customActionTooltip: "This is user custom action, it can use any settings!"
}
