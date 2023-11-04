pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property int baseSpacing: 5

    readonly property string mainTitle: "ChibiGame configurator"
    readonly property string trashCanImage: "qrc:qml/Images/trash_can.png"

    // Dark colors
    readonly property color darkTextColor: "#FFFFFF"
    readonly property color darkPageBGColor: "#18181b"
    readonly property color darkButtonBGColor: "#9147ff"
    readonly property color darkButtonHoveredBGColor: "#B78FF2"
    readonly property color darkButtonBorderColor: "#443659"
    readonly property color darkPanelBGColor: "#2D2D4A"
    readonly property color darkPanelBorderColor: "#8F8FF2"
    readonly property color darkNotificationBGColor: "#B78FF2"
    readonly property color darkNotificationBorderColor: "#7C728C"
    readonly property color darkSettingsItemBGColor: "#24243B"
    readonly property color darkSettingsItemBorderColor: "#363659"
    readonly property color darkImagePreviewBGColor: "#222238"
    readonly property color darkImagePreviewBorderColor: "#2D2D4A"
    readonly property color darkActionListItemBGColor: "#24243B"
    readonly property color darkActionListItemBorderColor: "#363659"
    readonly property color darkActionListItemBorderSelectedColor: "#54548A"

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
    readonly property color lightImagePreviewBGColor: "#222238"
    readonly property color lightImagePreviewBorderColor: "#2D2D4A"
    readonly property color lightActionListItemBGColor: "#474747"
    readonly property color lightActionListItemBorderColor: "#474747"
    readonly property color lightActionListItemBorderSelectedColor: "#474747"

    // Style
    // Colors
    readonly property bool bIsDarkTheme: true
    readonly property color textColor: bIsDarkTheme ? darkTextColor : lightTextColor
    readonly property color pageBGColor: bIsDarkTheme ? darkPageBGColor : lightPageBGColor
    readonly property color buttonBGColor: bIsDarkTheme ? darkButtonBGColor : lightButtonBGColor
    readonly property color buttonHoveredBGColor: bIsDarkTheme ? darkButtonHoveredBGColor : lightButtonHoveredBGColor
    readonly property color buttonBorderColor: bIsDarkTheme ? darkButtonBorderColor : lightButtonBorderColor
    readonly property color panelBGColor: bIsDarkTheme ? darkPanelBGColor : lightPanelBGColor
    readonly property color panelBorderColor: bIsDarkTheme ? darkPanelBGColor : lightPanelBorderColor
    readonly property color notificationBGColor: bIsDarkTheme ? darkNotificationBGColor : lightNotificationBGColor
    readonly property color notificationBorderColor: bIsDarkTheme ? darkNotificationBorderColor : lightNotificationBorderColor
    readonly property color settingsItemBGColor: bIsDarkTheme ? darkSettingsItemBGColor : lightSettingsItemBGColor
    readonly property color settingsItemBorderColor: bIsDarkTheme ? darkSettingsItemBorderColor : lightSettingsItemBorderColor
    readonly property color imagePreviewBGColor: bIsDarkTheme ? darkImagePreviewBGColor : lightImagePreviewBGColor
    readonly property color imagePreviewBorderColor: bIsDarkTheme ? darkImagePreviewBorderColor : lightImagePreviewBorderColor
    readonly property color actionListItemBGColor: bIsDarkTheme ? darkActionListItemBGColor : lightActionListItemBGColor
    readonly property color actionListItemBorderColor: bIsDarkTheme ? darkImagePreviewBorderColor : lightImagePreviewBorderColor
    readonly property color actionListItemBorderSelectedColor: bIsDarkTheme ? darkActionListItemBorderSelectedColor : lightActionListItemBorderSelectedColor

    // Sizes
    readonly property int buttonBorderSize: 2
    readonly property int buttonFontSize: 24
    readonly property int configurationButtonFontSize: 14
    readonly property int buttonRadiusSize: 10
    readonly property int textSize: 18
    readonly property int actionItemTextSize: 16

    // Margins
    readonly property int defaultMargin: 5

    // Settings
    readonly property real notificationOpacity: 0.75
}
