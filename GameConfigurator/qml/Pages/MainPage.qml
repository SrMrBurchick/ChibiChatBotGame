import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import ProcessesComponent

Item {
    id: menu
    anchors.fill: parent
    anchors.centerIn: parent

    signal openConfig()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            text: "Start game"
            onClicked: {
                ProcessManager.runGame()
            }
        }

        BaseButton {
            text: "Start chat bot"
        }

        BaseButton {
            id: configuration
            text: "Configure"
            onClicked: openConfig()
        }

        BaseButton {
            text: "Exit"
            onClicked: {
                Qt.quit()
            }

        }
    }
}

