import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs
import ConfigComponent
import Managers
import Base

Item {
    id: twitch_menu
    anchors.fill: parent
    anchors.centerIn: parent

    property StackView rootStack: StackView.view
    //
    // WorkerScript {
    //     id: worker
    //     source: "qrc:qml/pageLoader.mjs"
    //     onMessage: (messageObject)=> {
    //     }
    // }


    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            scaler: 1.5
            text: "Channel Points Configuration"
            onClicked: rootStack.push(channel_points_config)
        }

        BaseButton {
            scaler: 1.5
            text: "Back"
            onClicked: rootStack.pop()
        }
    }

    Component {
        id: channel_points_config
        ChannelPointsConfigurationPage {
            rootStack: twitch_menu.rootStack
        }
    }

    Component.onCompleted: {
    }

    signal goBack()
}
