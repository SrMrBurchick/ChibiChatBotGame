import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Pages

ApplicationWindow {
    visible: true

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: MainPage {
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
        }
    }
}
