import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Delegates
import Panels

// TODO: Refactor style
Item {
    id: root
    anchors.fill: parent
    anchors.margins: 10

    property StackView rootStack: StackView.view

    // Title
    Text {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Configuration"
        font.pointSize: 24
    }


    // Panel
    ColumnLayout {
        id: panel
        width: root.width
        anchors.top: title.bottom
        anchors.bottom: root.bottom
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "ChatBot"
            font.pointSize: 14
        }

        SettingsItemDelegate {
            fieldName: "Chanel name:"
            fieldDescription: "Twitch chanel target"
            onValueChanged:(text) => {
                ActionsManager.twitchChannel = text
            }
        }

        SettingsItemDelegate {
            fieldName: "Chat bot URL:"
            defaultText: ActionsManager.chatBotURL
            onValueChanged:(text) => {
                ActionsManager.chatBotURL = text
            }
        }

        SettingsItemDelegate {
            fieldName: "Chat bot port:"
            defaultText: ActionsManager.chatBotPort
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.chatBotPort = parseInt(text)
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Game"
            font.pointSize: 14
        }

        SettingsItemDelegate {
            fieldName: "Game screen width:"
            defaultText: ActionsManager.screenWidth
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.screenWidth = parseInt(text)
            }
        }

        SettingsItemDelegate {
            fieldName: "Game screen height:"
            defaultText: ActionsManager.screenHeight
            typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
            onValueChanged:(text) => {
                ActionsManager.screenHeight = parseInt(text)
            }
        }

        // Settings
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        // Controls
        RowLayout {
            Layout.fillWidth: true

            BaseButton {
                text: "Back"
                onClicked: {
                    rootStack.pop()
                }
            }

            Rectangle {
                Layout.fillWidth: true
            }

            BaseButton {
                text: "Save"
                onClicked: saveChatBotConfig()
            }

        }

    }

    signal saveChatBotConfig()
}

