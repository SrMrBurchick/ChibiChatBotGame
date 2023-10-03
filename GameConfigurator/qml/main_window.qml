import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Pages

ApplicationWindow {
    visible: true

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: actions_config
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
        }
    }

    Component {
        id: actions_config
        ActionsConfigurationPage {
            onGoBack: {
                stack.pop()
            }
        }
    }
}
