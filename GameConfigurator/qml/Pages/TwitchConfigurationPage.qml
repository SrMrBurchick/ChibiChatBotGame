import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs
import ConfigComponent
import Managers
import Base

Item {
    id: twitch_menu
    anchors.fill: parent
    anchors.centerIn: parent

    AuthorizationDialog {
        id: webView
    }

    property StackView rootStack: StackView.view

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            scaler: 1.5
            text: "Authorize"
            onClicked: {
                TwitchManager.authorize()
            }
        }

        BaseButton {
            scaler: 1.5
            text: "Channel Points Configuration"
            onClicked: {
                if (TwitchManager.isAuthorized()) {
                    rootStack.push(channel_points_config)
                } else {
                    NotificationsManager.notify("Twitch Authorization", "You're not authorized")
                }

            }
        }

        BaseButton {
            scaler: 1.5
            text: "Back"
            onClicked: {
                rootStack.pop()
            }
        }
    }

    Component {
        id: channel_points_config
        ChannelPointsConfigurationPage {
            rootStack: twitch_menu.rootStack
        }
    }

    Component.onCompleted: {
        TwitchManager.authorizationURLReady.connect(function (url){
            webView.setup(url)
            webView.open()
        })

        TwitchManager.connectionUpdated.connect(function (isConnected){
            if (isConnected) {
                TwitchManager.saveConfig(Config)
                Config.saveConfig()
            }
        })
    }

    signal goBack()
}
