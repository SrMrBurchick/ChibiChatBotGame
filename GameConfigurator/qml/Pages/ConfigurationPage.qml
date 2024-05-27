import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ActionsModels
import ConfigComponent
import Base

Item {
    id: configuration_menu
    anchors.fill: parent
    anchors.centerIn: parent


    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            scaler: 1.5
            text: "Setup Actions"
            onClicked: goActionsConfiguration()
        }

        BaseButton {
            scaler: 1.5
            text: "Common settings"
            onClicked: goChatBotConfiguration()
        }

        BaseButton {
            scaler: 1.5
            text: "Twitch settings"
            onClicked: goTwitchConfiguration()
        }

        BaseButton {
            scaler: 1.5
            text: "Back"
            onClicked: goBack()
        }
    }

    Component.onCompleted: {
        if (!Config.isConfigLoaded()) {
            Config.loadConfig()

            // if (!ActionsManager.isActionsConfigured) {
            //     Config.initActionsListModel(ActionsManager.actionsListModel)
            //     Config.initAnimationsSequenceModel(ActionsManager.sequenceModel)
            //     ActionsManager.isActionsConfigured = true
            // }
        }

        Config.onActionsConfigured.connect(function(){
            saveDefaultConfig()
        })

        Config.onGlobalSettingsConfigured.connect(function(){
            saveDefaultConfig()
        })
    }

    function saveDefaultConfig() {
        console.log("Save default")
        Config.saveChatBotConfig(GlobalConfig.chatBotURL, GlobalConfig.chatBotPort)
        Config.saveScreenResolution(GlobalConfig.screenHeight, GlobalConfig.screenWidth)
        Config.saveTargetTwitchChannel(GlobalConfig.twitchChannel)
        Config.saveActionExecutionTime(GlobalConfig.actionExecutionTime)
        Config.saveMessageTextColor(GlobalConfig.messageTextColor)
        Config.saveMovementSpeed(GlobalConfig.movementSpeed)
        Config.saveNextActionTimeout(GlobalConfig.nextActionTimeout)
        // Config.saveSpriteSheetPath(ActionsManager.spriteSheetPath)
        // Config.saveSpriteSettings(ActionsManager.spriteSizeWidth, ActionsManager.spriteSizeHeight)
        // Config.saveSpriteScale(ActionsManager.spriteScale)
        // Config.saveTableSettings(ActionsManager.tableSettingsColumns, ActionsManager.tableSettingsRows)

        Config.saveConfig()
    }

    signal goBack()
    signal goActionsConfiguration()
    signal goChatBotConfiguration()
    signal goTwitchConfiguration()
}
