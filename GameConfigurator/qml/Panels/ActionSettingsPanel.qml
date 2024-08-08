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
                defaultText: actionConfig ? actionConfig.text : ""
                onValueChanged:(text) => {
                    if (actionConfig) {
                        actionConfig.text = text
                    }
                }
            }

            PropertyBoolDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                fieldName: "Can Interrupt:"
                defaultValue: actionConfig ? actionConfig.canInterrupt : false
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.canInterrupt = value
                    }
                }
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
                defaultValue: actionConfig ? actionConfig.textColor : "red"
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.textColor = value
                    }
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 8
                maxValue: 72

                fieldName: "Font size:"
                defaultValue: actionConfig ? actionConfig.fontSize : 54
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.fontSize = value
                    }
                }
            }

            PropertyIntDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                minValue: 1
                maxValue: 20

                fieldName: "Text display time:"
                defaultValue: actionConfig ? actionConfig.displayTime : 10
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.displayTime = value
                    }
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

                function updateDisplayText() {
                    var action = ActionsManager.getSelectedAction();
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
                }

                Component.onCompleted: {
                    if (ActionsManager && TwitchManager) {
                        updateDisplayText()
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
            font.pixelSize : 32
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
                    settingsModel.clear()
                    actionConfig = action.getConfig()
                    actionName = action.name
                    if (!ActionsManager.isGameDefaultAction(action)) {
                        settingsModel.append({"name": "Base Settings", "component": baseSettings})
                        settingsModel.append({"name": "Text Settings", "component": textSettings})
                    }

                    if (!ActionsManager.isTwitchDefaultAction(action)) {
                        settingsModel.append({"name": "Twitch Settings", "component": twitchSettings})
                    }
                }
            })
        }
    }
}

