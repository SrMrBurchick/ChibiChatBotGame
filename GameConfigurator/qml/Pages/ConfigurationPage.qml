import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons

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

    signal goBack()
    signal goActionsConfiguration()
    signal goChatBotConfiguration()
}

