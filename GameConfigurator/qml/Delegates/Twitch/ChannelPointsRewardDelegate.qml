import QtQuick
import QtQuick.Layouts
import Base
import Delegates
import Panels
import Managers
import TwitchManagerComponent
import Buttons

BasePanel {
    id: root
    property ChannelPointsReward target
    readonly property int offset: 10
    height: content.height + 2 * offset
    radius: 15.0

    ColumnLayout {
        id: content
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: offset

        ColumnLayout {
            id: panel
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            PropertyDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                fieldName: "Title:"
                fieldDescription: "Custom reward title"
                defaultText: target ? target.title : ""
                onValueChanged:(text) => {
                    if (target) {
                        target.title = text
                    }
                }
            }

            PropertyIntDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                minValue:1
                maxValue:9999

                fieldName: "Cost:"
                fieldDescription: "Custom reward cost"
                defaultValue: target ? target.cost : 1
                onValueChanged:(cost) => {
                    if (target) {
                        target.cost = cost
                    }
                }
            }

            PropertyBoolDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                fieldName: "Enabled:"
                defaultValue: target ? target.enabled : false
                onValueChanged:(value) => {
                    if (target) {
                        target.enabled = value
                    }
                }
            }

            PropertyBoolDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                fieldName: "User input required:"
                defaultValue: target ? target.userInputRequired : false
                onValueChanged:(value) => {
                    if (target) {
                        target.userInputRequired = value
                    }
                }
            }

            Component {
                id: userInput
                PropertyDelegate {
                    defaultText: target ? target.userInputPrompt : ""
                    fieldName: "UserInput:"
                    fieldDescription: "Input description"
                    onValueChanged:(value) => {
                        if (target) {
                            target.userInputPrompt = value
                        }
                    }
                }
            }

            Loader {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: target ? target.userInputRequired : false
                sourceComponent: userInput
            }
        }

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            BaseButton {
                scaler: 1.0
                text: "Update"
                onClicked: {
                    if (TwitchManager) {
                        TwitchManager.updateChannelPointsReward(target)
                    }
                }
            }

            BaseButton {
                scaler: 1.0
                text: "Remove"
                onClicked: {
                    if (TwitchManager) {
                        TwitchManager.removeChannelPointsRewardById(reward_id)
                    }
                }
            }
        }
    }


    Component.onCompleted: {
        if (TwitchManager) {
            target = TwitchManager.getChannelPointRewardByID(reward_id)
        }
    }
}
