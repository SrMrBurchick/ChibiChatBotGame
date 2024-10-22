pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property int baseSpacing: 5

    readonly property string mainTitle: "ChibiGame configurator"

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
    readonly property color darkPropertyDelegateBGColor: "#24243B"
    readonly property color darkPropertyDelegateBorderColor: "#363659"
    readonly property color darkImagePreviewBGColor: "#222238"
    readonly property color darkImagePreviewBorderColor: "#2D2D4A"
    readonly property color darkActionListItemBGColor: "#24243B"
    readonly property color darkActionListItemBorderColor: "#363659"
    readonly property color darkActionListItemBorderSelectedColor: "#54548A"
    readonly property color darkSettingsDelegateBGColor: "#4B4B82"
    readonly property color darkSettingsDelegateBorderColor: "#4545B5"
    readonly property color darkSpriteItemSelectedBGColor: "#7C728C"
    readonly property color darkGameRunningInfoBGColor: "#F5A51E"
    readonly property color darkGameRunningInfoBorderColor: "#C2913E"
    readonly property color darkGameRunningInfoTextColor: "#FFFFFF"


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
    readonly property color lightPropertyDelegateBGColor: "#8b8b8b"
    readonly property color lightPropertyDelegateBorderColor: "#474747"
    readonly property color lightImagePreviewBGColor: "#222238"
    readonly property color lightImagePreviewBorderColor: "#2D2D4A"
    readonly property color lightActionListItemBGColor: "#474747"
    readonly property color lightActionListItemBorderColor: "#474747"
    readonly property color lightActionListItemBorderSelectedColor: "#474747"
    readonly property color lightSettingsDelegateBGColor: "#474747"
    readonly property color lightSettingsDelegateBorderColor: "#474747"
    readonly property color lightSpriteItemSelectedBGColor: "#474747"
    readonly property color lightGameRunningInfoBGColor: "#F5A51E"
    readonly property color lightGameRunningInfoBorderColor: "#C2913E"
    readonly property color lightGameRunningInfoTextColor: "#FFFFFF"

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
    readonly property color propertyDelegateBGColor: bIsDarkTheme ? darkPropertyDelegateBGColor : lightPropertyDelegateBGColor
    readonly property color propertyDelegateBorderColor: bIsDarkTheme ? darkPropertyDelegateBorderColor : lightPropertyDelegateBorderColor
    readonly property color imagePreviewBGColor: bIsDarkTheme ? darkImagePreviewBGColor : lightImagePreviewBGColor
    readonly property color imagePreviewBorderColor: bIsDarkTheme ? darkImagePreviewBorderColor : lightImagePreviewBorderColor
    readonly property color actionListItemBGColor: bIsDarkTheme ? darkActionListItemBGColor : lightActionListItemBGColor
    readonly property color actionListItemBorderColor: bIsDarkTheme ? darkImagePreviewBorderColor : lightImagePreviewBorderColor
    readonly property color actionListItemBorderSelectedColor: bIsDarkTheme ? darkActionListItemBorderSelectedColor : lightActionListItemBorderSelectedColor
    readonly property color settingsDelegateBGColor: bIsDarkTheme ? darkSettingsDelegateBGColor : lightSettingsDelegateBGColor
    readonly property color settingsDelegateBorderColor: bIsDarkTheme ? darkSettingsDelegateBorderColor : lightSettingsDelegateBorderColor
    readonly property color spriteItemSelectedBGColor: bIsDarkTheme ? darkSpriteItemSelectedBGColor : lightSpriteItemSelectedBGColor
    readonly property color gameRunningInfoBGColor: bIsDarkTheme ? darkGameRunningInfoBGColor : lightGameRunningInfoBGColor
    readonly property color gameRunningInfoBorderColor: bIsDarkTheme ? darkGameRunningInfoBorderColor : lightGameRunningInfoBorderColor
    readonly property color gameRunningInfoTextColor: bIsDarkTheme ? darkGameRunningInfoTextColor : lightGameRunningInfoTextColor

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

    readonly property string lightTrashCanImage: "qrc:qml/Images/trash_can.png"
    readonly property string darkTrashCanImage: "qrc:qml/Images/trash_can_dark_theme.png"
    readonly property string darkCopyImage: "qrc:qml/Images/copy_white.png"
    readonly property string lightCopyImage: "qrc:qml/Images/copy.png"
    readonly property string trashCanImage: bIsDarkTheme ? darkTrashCanImage : lightTrashCanImage
    readonly property string copyImage: bIsDarkTheme ? darkCopyImage : lightCopyImage
    readonly property string twitchIcon: "qrc:qml/Images/twitch-logo.png"
    readonly property string gearIcon: "qrc:qml/Images/gear_icon.png"
    readonly property string templateIcon: "qrc:qml/Images/template_icon.png"
    readonly property string questionIcon: bIsDarkTheme ? "qrc:qml/Images/question_dark.png" : "grc:qml/Images/question_white.png"

}
