import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Pages
import SystemTools
import Base

ApplicationWindow {
    visible: true

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: main_menu
    }

    NotificationListModel {
        id: notificationModel
    }

    ListView {
        height: 200
        width: 200
        x: parent.width - width
        anchors.margins: 5
        spacing: 5
        model: notificationModel
        delegate: Notification {
            description: title
            message: body
            rootModel: notificationModel
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
