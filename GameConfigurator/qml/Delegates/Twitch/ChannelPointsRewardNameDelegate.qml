import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Delegates
import Panels
import Managers
import TwitchManagerComponent
import Buttons

ItemDelegate {
    id: root
    property ChannelPointsReward target

    text: target ? target.title : "unknown"

    Component.onCompleted: {
        if (TwitchManager) {
            target = TwitchManager.getChannelPointRewardByID(reward_id)
        }
    }
}
