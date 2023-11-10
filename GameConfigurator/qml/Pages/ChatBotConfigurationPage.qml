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
                    onValueChanged:(text) => {
                        ActionsManager.twitchChannel = text
                    }
                }

                PropertyDelegate {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    fieldName: "Chat bot URL:"
                    defaultText: ActionsManager.chatBotURL
                    onValueChanged:(text) => {
                        ActionsManager.chatBotURL = text
                    }
                }

                PropertyDelegate {
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

                PropertyDelegate {
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

                PropertyDelegate {
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
                    delegate: PredefinedActionDelegate {
                        onRemoveElement: (index) => {
                            actionsModel.removeElement(index)
                        }
                    }
                    onCountChanged: {
                        predefinedColumn.height = 60 * count
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
        settingsModel.append({"name": "Predefined actions", "component": predefinedActions})

        if (Config.isConfigLoaded()) {
            ActionsManager.chatBotURL = Config.getChatBotURL()
            ActionsManager.chatBotPort = Config.getChatBotPort()
            ActionsManager.screenHeight = Config.getScreenHeight()
            ActionsManager.screenWidth = Config.getScreenWidth()
            ActionsManager.twitchChannel = Config.getTwitchTargeChannel()
            Config.initPredefinedActionsListModel(actionsModel)
        }
    }

    function saveChatBotConfig() {
        Config.saveChatBotConfig(ActionsManager.chatBotURL, ActionsManager.chatBotPort)
        Config.saveScreenResolution(ActionsManager.screenHeight, ActionsManager.screenWidth)
        Config.saveTargetTwitchChannel(ActionsManager.twitchChannel)
        Config.savePredefinedActions(actionsModel)
        Config.saveConfig()
    }
}
