import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import GameActions
import ConfigComponent

Item {
    id: configuration_menu
    anchors.fill: parent
    anchors.centerIn: parent


    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            text: "Setup Actions"
            onClicked: goActionsConfiguration()
        }

        BaseButton {
            text: "Setup ChatBot"
            onClicked: goChatBotConfiguration()
        }

        BaseButton {
            text: "Back"
            onClicked: goBack()
        }
    }

    Component.onCompleted: {
        if (Config.isConfigLoaded() == false) {
            Config.loadConfig()
        }
    }

    signal goBack()
    signal goActionsConfiguration()
    signal goChatBotConfiguration()
}
