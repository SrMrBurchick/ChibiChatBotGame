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
                defaultText: target.title
                onValueChanged:(text) => {
                }
            }

            PropertyIntDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                maxValue:9999

                fieldName: "Cost:"
                fieldDescription: "Custom reward cost"
                defaultValue: target.cost
                onValueChanged:(cost) => {
                }
            }

            PropertyBoolDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true

                fieldName: "Enabled:"
                defaultValue: target.enabled
                onValueChanged:(value) => {
                }
            }
        }

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            BaseButton {
                scaler: 1.0
                text: "Update"
                onClicked: {
                }
            }

            BaseButton {
                scaler: 1.0
                text: "Remove"
                onClicked: {
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
