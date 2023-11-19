import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import Pages
import SystemTools
import Base

ApplicationWindow {
    id: root
    visible: true
    title: "ChibiGame Configurator"
    visibility: Window.Maximized
    width: 1920
    height: 1080
    background: Rectangle {
        color: Style.pageBGColor
    }

    SystemTrayIcon {
        visible: true
        // TODO: Draw tray icon
        // icon.source: "Docs/icon_bg.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Run Game") // TODO: Update title
            }
            MenuItem {
                text: qsTr("Run ChatBot") // TODO: Update title
            }
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: main_menu
    }

    NotificationListModel {
        id: notificationModel
        onNotificationsCountChanged: (count) => {
            notificationsList.height = count * 200
        }
    }

    Component {
        id: notification
        Notification {
            description: title
            message: body
            rootModel: notificationModel
        }
    }

    ListView {
        property real maxWidth: 250
        id: notificationsList
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 5
        anchors.topMargin: 5
        width: maxWidth
        spacing: 5
        model: notificationModel
        delegate: notification
        clip: true
    }

    Rectangle {
        anchors.fill: parent
        opacity: 0.8
        visible: busy.running
        color: "black"
        MouseArea {
            anchors.fill: parent
            onClicked: {

            }
        }
    }

    ChibiBusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: GlobalConfig.isBusy
    }

    WorkerScript {
        id: worker
        source: "qrc:qml/pageLoader.mjs"
        onMessage: (messageObject)=> {
            if (messageObject.reply == "ActionsConfig") {
                stack.push(actions_config)
            } else if (messageObject.reply == "CommonConfig") {
                stack.push(chatBotConfig)
            }
        }
    }

    Component {
        id: main_menu
        MainPage {
            onOpenConfig: {
                stack.push(config_menu)
            }
        }
    }

    Component {
        id: config_menu
        ConfigurationPage {
            onGoBack: {
                stack.pop()
            }
            onGoActionsConfiguration: {
                GlobalConfig.isBusy = true
                worker.sendMessage({'page': "ActionsConfig"})
            }
            onGoChatBotConfiguration: {
                GlobalConfig.isBusy = true
                worker.sendMessage({'page': "CommonConfig"})
            }
        }
    }

    Component {
        id: actions_config
        ActionsConfigurationPage {
            rootStack: stack
        }
    }

    Component {
        id: chatBotConfig
        ChatBotConfigurationPage {
            rootStack: stack
        }
    }

    Component.onCompleted: {
        NotificationsManager.addModel(notificationModel);
    }
}
