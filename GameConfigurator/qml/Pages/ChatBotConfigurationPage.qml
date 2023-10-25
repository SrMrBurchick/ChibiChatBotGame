import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Delegates
import Panels
import ConfigComponent
import Base

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 10

    property StackView rootStack: StackView.view

    // Title
    BaseText {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Configuration"
        font.pointSize: 32
        font.bold: true
    }


    // Panel
    ColumnLayout {
        id: panel
        width: root.width
        anchors.top: title.bottom
        anchors.bottom: root.bottom
        BasePanel {
            Layout.fillWidth: true
            height: chat_bot_settings.height

            ColumnLayout {
                id: chat_bot_settings
                width: parent.width

                BaseText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "ChatBot"
                    font.pointSize: 14
                    font.bold: true
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

            }
        }


        BasePanel {
            Layout.fillWidth: true
            height: game_settings.height

            ColumnLayout {
                id: game_settings
                width: parent.width

                BaseText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Game"
                    font.pointSize: 14
                    font.bold: true
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

            }
        }

        // Settings
        Rectangle {
            opacity: 0
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
                opacity: 0
                Layout.fillWidth: true
            }

            BaseButton {
                text: "Save"
                onClicked: saveChatBotConfig()
            }

        }

    }

    Component.onCompleted: {
        if (Config.isConfigLoaded()) {
            ActionsManager.chatBotURL = Config.getChatBotURL()
            ActionsManager.chatBotPort = Config.getChatBotPort()
        }
    }

    signal saveChatBotConfig()
}

