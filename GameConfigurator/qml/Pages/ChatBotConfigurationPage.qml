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

        Component {
            id: chatBotSettings
            ColumnLayout {
                width: parent.width

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

        Component {
            id: systemSettings
            ColumnLayout {
                width: parent.width

                SettingsItemDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    fieldName: "screen width:"
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

                    fieldName: "screen height:"
                    defaultText: ActionsManager.screenHeight
                    typeValidator: RegularExpressionValidator{regularExpression: /^[0-9,/]+$/}
                    onValueChanged:(text) => {
                        ActionsManager.screenHeight = parseInt(text)
                    }
                }
            }
        }

        ListModel {
            id: settingsModel
        }

        BasePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                ListView {
                    width: parent.width
                    spacing: 10
                    model: settingsModel

                    delegate: Rectangle {
                        id: delegate
                        property bool isExpanded: false
                        color: Style.settingsDelegateBGColor
                        border.color: Style.settingsDelegateBorderColor
                        border.width: 2
                        width: parent.width
                        height: isExpanded ? content.height + contentLoader.height + 5 : content.height
                        RowLayout {
                            id: content
                            spacing: 10
                            BaseText {
                                id: description
                                text: name
                                font.pixelSize: 32
                                font.bold: true
                            }

                            BaseText {
                                text: delegate.isExpanded ? "▼" : "►"
                                font.pixelSize: 32
                                font.bold: true

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        delegate.isExpanded = !delegate.isExpanded
                                    }
                                }
                            }
                        }

                        Loader {
                            id: contentLoader
                            visible: delegate.isExpanded
                            anchors.top: content.bottom
                            width: delegate.width
                            sourceComponent: component
                        }
                    }
                }
            }
        }

        // Controls
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 5

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

            BaseButton {
                text: "Back"
                Layout.fillWidth: true
                Layout.maximumWidth: 400
                onClicked: {
                    rootStack.pop()
                }
            }

            BaseButton {
                text: "Save"
                Layout.fillWidth: true
                Layout.maximumWidth: 400
                onClicked: saveChatBotConfig()
            }

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

        }

    }

    Component.onCompleted: {
        settingsModel.append({"name": "System Settings", "component": systemSettings})
        settingsModel.append({"name": "ChatBot Settings", "component": chatBotSettings})

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

