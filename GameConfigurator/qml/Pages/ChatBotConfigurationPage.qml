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


    ColumnLayout {
        anchors.fill: root

        // Title
        BaseText {
            id: title
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Configuration"
            font.pointSize: 32
            font.bold: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Chat bot settings
            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

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
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 10

                        fieldName: "Chanel name:"
                        fieldDescription: "Twitch chanel target"
                        onValueChanged:(text) => {
                            ActionsManager.twitchChannel = text
                        }
                    }

                    SettingsItemDelegate {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 10

                        fieldName: "Chat bot URL:"
                        defaultText: ActionsManager.chatBotURL
                        onValueChanged:(text) => {
                            ActionsManager.chatBotURL = text
                        }
                    }

                    SettingsItemDelegate {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 10

                        fieldName: "Chat bot port:"
                        defaultText: ActionsManager.chatBotPort
                        typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
                        onValueChanged:(text) => {
                            ActionsManager.chatBotPort = parseInt(text)
                        }
                    }

                }
            }

            // Game settings
            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                // height: game_settings.height

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
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 10

                        fieldName: "Game screen width:"
                        defaultText: ActionsManager.screenWidth
                        typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
                        onValueChanged:(text) => {
                            ActionsManager.screenWidth = parseInt(text)
                        }
                    }

                    SettingsItemDelegate {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 10

                        fieldName: "Game screen height:"
                        defaultText: ActionsManager.screenHeight
                        typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
                        onValueChanged:(text) => {
                            ActionsManager.screenHeight = parseInt(text)
                        }
                    }

                }
            }
        }

        // Controls
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 5

            BaseButton {
                text: "Back"
                Layout.fillWidth: true
                onClicked: {
                    rootStack.pop()
                }
            }

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

            BaseButton {
                text: "Save"
                Layout.fillWidth: true
                onClicked: saveChatBotConfig()
            }

        }

    }

    Component.onCompleted: {
        if (Config.isConfigLoaded()) {
            ActionsManager.chatBotURL = Config.getChatBotURL()
            ActionsManager.chatBotPort = Config.getChatBotPort()
            ActionsManager.screenHeight = Config.getScreenHeight()
            ActionsManager.screenWidth = Config.getScreenWidht()
            ActionsManager.twitchChannel = Config.getTwitchTargeChannel()
        }
    }

    function saveChatBotConfig() {
        Config.saveChatBotConfig(ActionsManager.chatBotURL, ActionsManager.chatBotPort)
        Config.saveScreenResolution(ActionsManager.screenHeight, ActionsManager.screenWidth)
        Config.saveTargetTwitchChannel(ActionsManager.twitchChannel)
        Config.saveConfig()
    }
}

