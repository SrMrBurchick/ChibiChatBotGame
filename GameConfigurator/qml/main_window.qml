import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import Pages
import SystemTools
import Base

ApplicationWindow {
    visible: true
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
        id: notificationsList
        anchors.right: parent.right
        anchors.margins: 5
        width: 200
        spacing: 5
        model: notificationModel
        delegate: notification
        clip: true
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
                stack.push(actions_config)
            }
            onGoChatBotConfiguration: {
                stack.push(chatBotConfig)
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
