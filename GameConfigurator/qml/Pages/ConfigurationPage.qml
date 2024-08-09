import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ActionsModels
import ConfigComponent
import Base
import Managers

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
        TwitchManager.onConnectionUpdated.connect(function (isConnected) {
            TwitchManager.saveConfig(Config)
        })
        if (!Config.isLoaded) {
            Config.loadConfig()

            ActionsManager.initByConfig(Config)
            TwitchManager.initByConfig(Config)
        }
    }

    signal goBack()
    signal goActionsConfiguration()
    signal goChatBotConfiguration()
    signal goTwitchConfiguration()
}
