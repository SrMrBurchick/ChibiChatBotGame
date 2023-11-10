pragma Singleton
import QtQuick

QtObject {
    // System settings
    property string twitchChannel
    property string chatBotURL: "localhost"
    property int chatBotPort: 6565
    property int screenWidth: 1920
    property int screenHeight: 1200
    property real actionExecutionTime: 10 // seconds
}