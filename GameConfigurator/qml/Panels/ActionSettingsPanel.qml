import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ConfigComponent
import Delegates
import Base
import Managers
import TwitchModels
import ActionsManagerComponent

Item {
    id: root
    anchors.fill: parent
    anchors.leftMargin: 6
    property ActionConfig actionConfig
    property string actionName

    Component {
        id: baseSettings
        ColumnLayout {
            width: parent.width

            PropertyDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Text:"
                fieldDescription: "Text that should appear if action is execute"
                defaultText: GlobalConfig.twitchChannel
                onValueChanged:(text) => {
                    // GlobalConfig.twitchChannel = text
                }
            }

            PropertyBoolDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Can Interrupt:"
                defaultValue: false
                onValueChanged:(value) => {
                    // Config.saveChatBotUser(value)
                }
            }

            PropertyBoolDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Custom Animation:"
                defaultValue: false
                onValueChanged:(value) => {
                    // Config.saveChatBotUser(value)
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 1
                maxValue: 20

                fieldName: "Text display time:"
                // defaultValue: GlobalConfig.screenHeight
                // onValueChanged:(value) => {
                //     GlobalConfig.screenHeight = value
                // }
            }


        }
    }

    Component {
        id: textSettings
        ColumnLayout {
            width: parent.width

            PropertyColorDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Color:"
                defaultValue: GlobalConfig.messageTextColor
                onValueChanged:(value) => {
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 8
                maxValue: 72

                fieldName: "Font size:"
                defaultValue: GlobalConfig.fontSize
                onValueChanged:(value) => {
                }
            }

        }
    }

    ChannelPointsRewardsModel {
        id: rewardsModel
    }


    Component {
        id: twitchSettings
        ColumnLayout {
            width: parent.width

            PropertyComboBoxDelegate {
                id: rewardsComboBox
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Listen:"
                targetModel: rewardsModel
                defaultValue: actionConfig.rewardID

                targetDelegate: Component {
                    ChannelPointsRewardNameDelegate {}
                }

                onValueChanged:(value) => {
                    console.log("Selected: ", value)

                    if (actionConfig) {
                        actionConfig.rewardID = value
                    }
                }

                Component.onCompleted: {
                    if (ActionsManager && TwitchManager) {
                        ActionsManager.onActionSelected.connect(function(action) {
                            var displayText = ""
                            if (action) {
                                var config = action.getConfig()
                                if (config) {
                                    var reward = TwitchManager.getChannelPointRewardByID(config.rewardID)
                                    if (reward) {
                                        displayText = reward.title
                                    }
                                }
                            }

                            setDisplayText(displayText)
                        })
                    }
                }
            }
        }
    }

    ListModel {
        id: settingsModel
    }

    ColumnLayout {
        anchors.fill: root
        anchors.margins: 10
        BaseText {
            text: "Config: " + actionName
            font.pixelSize : 54
        }
        ScrollView {
            id: panel
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: settingsView
                width: parent.width
                spacing: 10
                model: settingsModel

                delegate: SettingsDelegate {
                    isEnabled: actionConfig ? true : false
                    onComponentLoaded: (component) => {
                        console.log("Component loaded ==>", component.comboBox)
                    }
                }
            }
        }

    }

    Component.onCompleted: {
        if (TwitchManager) {
            TwitchManager.requestChannelPointsRewards()
            rewardsModel.subscribeOnTarget(TwitchManager)
        }

        if (ActionsManager) {
            ActionsManager.onActionSelected.connect(function(action) {
                console.log("Selected Action: ", action)
                if (action) {
                    actionConfig = action.getConfig()
                    actionName = action.name
                }
            })
        }

        settingsModel.append({"name": "Base Settings", "component": baseSettings})
        settingsModel.append({"name": "Text Settings", "component": textSettings})
        settingsModel.append({"name": "Twitch Settings", "component": twitchSettings})
    }
}

