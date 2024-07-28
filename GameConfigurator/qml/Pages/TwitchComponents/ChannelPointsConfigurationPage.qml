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
import TwitchModels

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

        // Data
        ListView {
            id:rewardsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            model: ChannelPointsRewardsModel {}

            delegate: ChannelPointsRewardDelegate {
                width: parent.width / 2
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        BaseButton {
            text: "+"
            Layout.fillWidth: true
            Layout.maximumWidth: 400
            onClicked: {
                if (TwitchManager && TwitchManager.isCanCreateNewEmptyReward()) {
                    TwitchManager.createNewChannelPointsReward()
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

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

        }

    }

    Component.onCompleted: {
        TwitchManager.requestChannelPointsRewards()
        rewardsList.model.subscribeOnTarget(TwitchManager)
    }

}
