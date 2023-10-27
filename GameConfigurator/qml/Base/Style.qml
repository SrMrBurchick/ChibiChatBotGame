pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property int baseSpacing: 5

    readonly property string mainTitle: "ChibiGame configurator"
    readonly property string trashCanImage: "qrc:qml/Images/trash_can.png"

    // Dark colors
    readonly property color darkTextColor: "#8b8b8b"
    readonly property color darkPageBGColor: "#1d1a21"
    readonly property color darkButtonBGColor: "#393342"
    readonly property color darkButtonHoveredBGColor: "#534b60"
    readonly property color darkButtonBorderColor: "#2d2a31"
    readonly property color darkPanelBGColor: "#4d4852"
    readonly property color darkPanelBorderColor: "#1f1d21"
    readonly property color darkNotificationBGColor: "#2c2731"
    readonly property color darkNotificationBorderColor: "#1f1d21"
    readonly property color darkSettingsItemBGColor: "#342f3b"
    readonly property color darkSettingsItemBorderColor: "#1f1d21"

    // Light colors
    readonly property color lightTextColor: "black"
    readonly property color lightPageBGColor: "white"
    readonly property color lightButtonBGColor: "#818181"
    readonly property color lightButtonHoveredBGColor: "#8b8b8b"
    readonly property color lightButtonBorderColor: "#474747"
    readonly property color lightPanelBGColor: "#cfcfcf"
    readonly property color lightPanelBorderColor: "#7a7a7a"
    readonly property color lightNotificationBGColor: "#cfcfcf"
    readonly property color lightNotificationBorderColor: "#7a7a7a"
    readonly property color lightSettingsItemBGColor: "#8b8b8b"
    readonly property color lightSettingsItemBorderColor: "#474747"

    // Style
    // Colors
    readonly property bool bIsDarkTheme: true
    readonly property color textColor: bIsDarkTheme ? darkTextColor : lightTextColor
    readonly property color pageBGColor: bIsDarkTheme ? darkPageBGColor : lightPageBGColor
    readonly property color buttonBGColor: bIsDarkTheme ? darkButtonBGColor : lightButtonBGColor
    readonly property color buttonHoveredBGColor: bIsDarkTheme ? darkButtonHoveredBGColor : lightButtonHoveredBGColor
    readonly property color buttonBorderColor: bIsDarkTheme ? darkButtonBorderColor : lightButtonBorderColor
    readonly property color panelBGColor: bIsDarkTheme ? darkPageBGColor : lightPanelBGColor
    readonly property color panelBorderColor: bIsDarkTheme ? darkPanelBGColor : lightPanelBorderColor
    readonly property color notificationBGColor: bIsDarkTheme ? darkNotificationBGColor : lightNotificationBGColor
    readonly property color notificationBorderColor: bIsDarkTheme ? darkNotificationBorderColor : lightNotificationBorderColor
    readonly property color settingsItemBGColor: bIsDarkTheme ? darkSettingsItemBGColor : lightSettingsItemBGColor
    readonly property color settingsItemBorderColor: bIsDarkTheme ? darkSettingsItemBorderColor : lightSettingsItemBorderColor

    // Sizes
    readonly property int buttonBorderSize: 2
    readonly property int buttonFontSize: 24
    readonly property int configurationButtonFontSize: 14
    readonly property int buttonRadiusSize: 10
    readonly property int textSize: 18

    // Margins
    readonly property int defaultMargin: 5

    // Settings
    readonly property real notificationOpacity: 0.75
}
