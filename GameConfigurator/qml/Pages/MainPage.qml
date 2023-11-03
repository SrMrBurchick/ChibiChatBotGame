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
            id: game
            text: ProcessManager.isGameRunning() ? "Stop game" : "Start game"
            onClicked: {
                if (ProcessManager.isGameRunning()) {
                    ProcessManager.stopGameRunning()
                } else {
                    ProcessManager.runGame()
                }

                game.text = ProcessManager.isGameRunning() ? "Stop game" : "Start game"
            }
        }

        BaseButton {
            id: chat_bot
            text: ProcessManager.isChatBotRunning() ? "Stop chat bot" : "Start chat bot"
            onClicked: {
                if (ProcessManager.isChatBotRunning()) {
                    ProcessManager.stopChatBotRunning();
                } else {
                    ProcessManager.runChatBot()
                }
                chat_bot.text = ProcessManager.isChatBotRunning() ? "Stop chat bot" : "Start chat bot"
            }
        }

        BaseButton {
            id: configuration
            text: "Configure"
            onClicked: openConfig()
        }

        BaseButton {
            text: "Exit"
            onClicked: {
                ProcessManager.killAll()
                Qt.quit()
            }

        }
    }
}

