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
import Managers

Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view


    ColumnLayout {
        anchors.fill: root
        anchors.margins: 50
        anchors.bottomMargin: 0

        ListModel {
            id: settingsModel
        }


        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            ListView {
                anchors.fill: parent
                spacing: 50
                model: settingsModel

                delegate: SettingsPanelDelegate {
                    width: parent.width / 2
                    anchors.horizontalCenter: parent.horizontalCenter
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
                onClicked: saveBotConfig()
            }

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

        }

    }


    Component {
        id: twitchBotSettings
        ColumnLayout {
            width: parent.width

            PropertyDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                isEditable: false

                fieldName: "Chanel name:"
                fieldDescription: "Twitch chanel target"
                defaultText: TwitchManager.channelName
            }

            PropertyDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Twitch bot URL:"
                defaultText: Config.systemConfig.twitchSettings.bot.url
                onValueChanged:(text) => {
                    Config.systemConfig.twitchSettings.bot.url = value
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 1111
                maxValue: 6565

                fieldName: "Twitch bot port:"
                defaultValue: Config.systemConfig.twitchSettings.bot.port
                onValueChanged:(value) => {
                    Config.systemConfig.twitchSettings.bot.port = value
                }
            }
        }
    }

    Component {
        id: systemSettings
        ColumnLayout {
            width: parent.width

            PropertyBoolDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Logger:"
                defaultValue: Config.systemConfig.logger
                onValueChanged:(value) => {
                    Config.setLoggerEnabled(value)
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 800
                maxValue: 2560

                fieldName: "Screen width:"
                defaultValue: Config.systemConfig.gameSettings.screenWidth
                onValueChanged:(value) => {
                    Config.systemConfig.gameSettings.screenWidth = value
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 600
                maxValue: 1440

                fieldName: "Screen height:"
                defaultValue: Config.systemConfig.gameSettings.screenHeight
                onValueChanged:(value) => {
                    Config.systemConfig.gameSettings.screenHeight = value
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

                        onChangeChance: (index, chance) => {
                            actionsModel.updateChance(index, chance)
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
                defaultValue: Config.systemConfig.gameSettings.actionExecutionTime
                onValueChanged:(value) => {
                    console.log("Update action execution time: ", value)
                    Config.systemConfig.gameSettings.actionExecutionTime = value
                }
            }

            PropertyColorDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Message color:"
                defaultValue: Config.systemConfig.messageSettings.messageColor
                onValueChanged:(value) => {
                    Config.systemConfig.messageSettings.messageColor = value
                }
            }

            PropertyColorDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Message border color:"
                defaultValue: Config.systemConfig.messageSettings.messageBorderColor
                onValueChanged:(value) => {
                    Config.systemConfig.messageSettings.messageBorderColor = value
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 8
                maxValue: 72

                fieldName: "Font size:"
                defaultValue: Config.systemConfig.messageSettings.fontSize
                onValueChanged:(value) => {
                    Config.systemConfig.messageSettings.fontSize = value
                }
            }

            PropertyRealDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 1000.0
                maxValue: 9999.0

                fieldName: "Movement speed:"
                defaultValue: Config.systemConfig.gameSettings.movementSpeed
                onValueChanged:(value) => {
                    Config.systemConfig.gameSettings.movementSpeed = value
                }
            }

            PropertyRealDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 5.0
                maxValue: 1800.0

                fieldName: "[AI] Timeout to generate next action:"
                defaultValue: Config.systemConfig.gameSettings.nextActionTimeout
                onValueChanged:(value) => {
                    Config.systemConfig.gameSettings.nextActionTimeout
                }
            }
        }
    }

    Component.onCompleted: {
        settingsModel.append({"name": "System Settings", "component": systemSettings})
        settingsModel.append({"name": "Twitch Settings", "component": twitchBotSettings})
        settingsModel.append({"name": "Game settings", "component": gameSettings})
        settingsModel.append({"name": "Predefined actions", "component": predefinedActions})
    }

    function saveBotConfig() {
        Config.saveConfig()
    }
}
