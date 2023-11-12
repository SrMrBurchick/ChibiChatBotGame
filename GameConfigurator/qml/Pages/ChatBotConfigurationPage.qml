import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Delegates
import Panels
import ConfigComponent
import Dialogs
import Base
import ConfigTools

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

                PropertyDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    fieldName: "Chanel name:"
                    fieldDescription: "Twitch chanel target"
                    defaultText: GlobalConfig.twitchChannel
                    onValueChanged:(text) => {
                        GlobalConfig.twitchChannel = text
                    }
                }

                PropertyDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    fieldName: "Chat bot URL:"
                    defaultText: GlobalConfig.chatBotURL
                    onValueChanged:(text) => {
                        GlobalConfig.chatBotURL = text
                    }
                }

                PropertyIntDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 1111
                    maxValue: 9999

                    fieldName: "Chat bot port:"
                    defaultValue: GlobalConfig.chatBotPort
                    onValueChanged:(value) => {
                        GlobalConfig.chatBotPort = value
                    }
                }
            }
        }

        Component {
            id: systemSettings
            ColumnLayout {
                width: parent.width

                PropertyIntDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 800
                    maxValue: 2560

                    fieldName: "screen width:"
                    defaultValue: GlobalConfig.screenWidth
                    onValueChanged:(value) => {
                        GlobalConfig.screenWidth = value
                    }
                }

                PropertyIntDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 600
                    maxValue: 1440

                    fieldName: "screen height:"
                    defaultValue: GlobalConfig.screenHeight
                    onValueChanged:(value) => {
                        GlobalConfig.screenHeight = value
                    }
                }
            }
        }

        PredefinedActionsModel {
            id: actionsModel
        }

        Component {
            id: predefinedActions
            ColumnLayout {
                id: predefinedColumn
                width: parent.width

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: actionsModel
                    spacing: 10
                    Component {
                        id: predefinedDelegate
                        PredefinedActionDelegate {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: 10

                            onRemoveElement: (index) => {
                                actionsModel.removeElement(index)
                            }
                        }
                    }
                    delegate: predefinedDelegate

                    onCountChanged: {
                        predefinedColumn.height = contentItem.children[0].height * count
                        predefinedColumn.height += spacing * count
                        predefinedColumn.height += addActionButton.height
                    }
                }

                BaseButton {
                    id: addActionButton
                    text: "Add action"
                    Layout.fillWidth: true
                    Layout.maximumWidth: 400
                    AddPredefinedActionDialog {
                        id: addActionDialog
                        onAddNewAction: (newAction, chance) => {
                            console.log("Add new action: ", newAction)

                            actionsModel.addNewAction(newAction, chance);
                        }

                        Component.onCompleted: {
                            actionsModel.onActionAdded.connect(function(action) {
                                addActionDialog.onActionAdded(action)
                            })
                            actionsModel.onActionRemoved.connect(function(action) {
                                addActionDialog.onActionRemoved(action)
                            })
                        }
                    }

                    onClicked: {
                        addActionDialog.setupDialog(actionsModel.getActions())
                        addActionDialog.open()
                    }
                }
            }
        }

        Component {
            id: gameSettings
            ColumnLayout {
                width: parent.width

                PropertyRealDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 5.0
                    maxValue: 15.0

                    fieldName: "Action execution time(seconds):"
                    defaultValue: GlobalConfig.actionExecutionTime
                    onValueChanged:(value) => {
                        console.log("Update action execution time: ", value)
                        GlobalConfig.actionExecutionTime = value
                    }
                }

                PropertyColorDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    fieldName: "Message color:"
                    defaultValue: GlobalConfig.messageTextColor
                    onValueChanged:(value) => {
                        GlobalConfig.messageTextColor = value
                    }
                }

                PropertyRealDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 1000.0
                    maxValue: 9999.0

                    fieldName: "Movement speed:"
                    defaultValue: GlobalConfig.movementSpeed
                    onValueChanged:(value) => {
                        GlobalConfig.movementSpeed = value
                    }
                }

                PropertyRealDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10
                    minValue: 5.0
                    maxValue: 1800.0

                    fieldName: "[AI] Timeout to generate next action:"
                    defaultValue: GlobalConfig.nextActionTimeout
                    onValueChanged:(value) => {
                        GlobalConfig.nextActionTimeout = value
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

                    delegate: SettingsDelegate {
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
        settingsModel.append({"name": "Game settings", "component": gameSettings})
        settingsModel.append({"name": "Predefined actions", "component": predefinedActions})

        if (Config.isConfigLoaded()) {
            GlobalConfig.chatBotURL = Config.getChatBotURL()
            GlobalConfig.chatBotPort = Config.getChatBotPort()
            GlobalConfig.screenHeight = Config.getScreenHeight()
            GlobalConfig.screenWidth = Config.getScreenWidth()
            GlobalConfig.twitchChannel = Config.getTwitchTargeChannel()
            GlobalConfig.actionExecutionTime = Config.getActionExecutionTime()
            GlobalConfig.messageTextColor = Config.getMessageTextColor()
            GlobalConfig.movementSpeed = Config.getMovementSpeed()
            GlobalConfig.nextActionTimeout = Config.getNextActionTimeout()
            Config.initPredefinedActionsListModel(actionsModel)
        }
    }

    function saveChatBotConfig() {
        Config.saveChatBotConfig(GlobalConfig.chatBotURL, GlobalConfig.chatBotPort)
        Config.saveScreenResolution(GlobalConfig.screenHeight, GlobalConfig.screenWidth)
        Config.saveTargetTwitchChannel(GlobalConfig.twitchChannel)
        Config.savePredefinedActions(actionsModel)
        Config.saveActionExecutionTime(GlobalConfig.actionExecutionTime)
        Config.saveMessageTextColor(GlobalConfig.messageTextColor)
        Config.saveMovementSpeed(GlobalConfig.movementSpeed)
        Config.saveNextActionTimeout(GlobalConfig.nextActionTimeout)

        if (Config.isConfigLoaded() == false) {
            Config.globalSettingsConfigured()
        } else {
            Config.saveConfig()
        }
    }
}
