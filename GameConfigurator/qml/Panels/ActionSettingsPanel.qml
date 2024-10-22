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
import Dialogs

Item {
    id: root
    anchors.fill: parent
    property ActionConfig actionConfig
    property string actionName

    Component {
        id: helperDelegate
        Rectangle {
            color: "transparent"
            width: helperKey.width + helperValue.width + 40
            height: 50
            RowLayout {
                anchors.fill: parent
                spacing: 5
                BaseText {
                    id: helperKey
                    text: key
                    color: Style.textColor
                    font.pixelSize: 40
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                }
                BaseText {
                    text: " - "
                    color: Style.textColor
                    font.pixelSize: 40
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                }

                BaseText {
                    id: helperValue
                    text: value
                    color: Style.textColor
                    font.pixelSize: 40
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                }
            }
        }
    }

    ListModel {
        id: helperContent
        ListElement {
            key: "{user}"
            value: "NickName of the user that invokes reward, or banned/unbanend/moderAdd/moderremoveuser. Or raider channel name"
        }
        ListElement {
            key: "{admin}"
            value: "For ban, name of the admin that was granted ban/unban/modderadd/moderremove"
        }
        ListElement {
            key: "{timeout}"
            value: "Timeout for ban/unban"
        }
        ListElement {
            key: "{gifter}"
            value: "User nickname that was gift a sub"
        }
        ListElement {
            key: "{viewers}"
            value: "Raid viewers count"
        }
        ListElement {
            key: "{message}"
            value: "User input"
        }
    }

    ShowHelpDialog {
        id: helpDialog
        customDelegate: helperDelegate
        customModel: helperContent
    }

    Component {
        id: baseSettings
        ColumnLayout {
            width: parent.width

            PropertyDelegate {
                Layout.fillWidth: true
                Layout.margins: 10

                isHelp: true
                fieldName: "Text:"
                fieldDescription: "Text that should appear if action is execute"
                defaultText: actionConfig ? actionConfig.text : ""
                onValueChanged:(text) => {
                    if (actionConfig) {
                        actionConfig.text = text
                    }
                }
                onHelpClicked:{
                    helpDialog.open()
                }
            }

            PropertyBoolDelegate {
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.topMargin: 0

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
                Layout.fillWidth: true
                Layout.margins: 10

                fieldName: "Color:"
                defaultValue: actionConfig ? actionConfig.textColor : "red"
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.textColor = value
                    }
                }
            }

            PropertyColorDelegate {
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.topMargin: 0

                fieldName: "Border Color:"
                defaultValue: actionConfig ? actionConfig.textBorderColor : "black"
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.textBorderColor = value
                    }
                }
            }

            PropertyIntDelegate {
                minValue: 8
                maxValue: 72
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.topMargin: 0

                fieldName: "Font size:"
                defaultValue: actionConfig ? actionConfig.fontSize : 54
                onValueChanged:(value) => {
                    if (actionConfig) {
                        actionConfig.fontSize = value
                    }
                }
            }

            PropertyIntDelegate {
                minValue: 1
                maxValue: 20
                Layout.fillWidth: true
                Layout.margins: 10
                Layout.topMargin: 0
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
                Layout.fillWidth: true
                Layout.margins: 10

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
            Layout.maximumWidth: parent.width
            wrapMode: Text.WordWrap
            font.pixelSize : 32
        }
        ScrollView {
            id: panel
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: root.width
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

