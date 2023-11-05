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
            id: gameButton
            scaler: 1.5
            property bool isRunning: false
            text: isRunning ? "Stop game" : "Start game"
            onClicked: {
                gameButton.enabled = false
                if (isRunning) {
                    ProcessManager.stopGameRunning()
                } else {
                    ProcessManager.runGame()
                }
            }
        }

        BaseButton {
            id: chatBotButton
            scaler: 1.5
            property bool isRunning: false
            text: isRunning ? "Stop chat bot" : "Start chat bot"
            onClicked: {
                chatBotButton.enabled = false
                if (ProcessManager.isChatBotRunning()) {
                    ProcessManager.stopChatBotRunning();
                } else {
                    ProcessManager.runChatBot()
                }
            }
        }

        BaseButton {
            id: configuration
            scaler: 1.5
            text: "Configure"
            onClicked: openConfig()
        }

        BaseButton {
            text: "Exit"
            scaler: 1.5
            onClicked: {
                ProcessManager.killAll()
                Qt.quit()
            }

        }
    }

    Component.onCompleted: {
        ProcessManager.onGameStarted.connect(function() {
            gameButton.isRunning = true
            gameButton.enabled = true
        })
        ProcessManager.onGameEnded.connect(function() {
            gameButton.isRunning = false
            gameButton.enabled = true
        })

        ProcessManager.onChatBotStarted.connect(function() {
            chatBotButton.isRunning = true
            chatBotButton.enabled = true
        })
        ProcessManager.onChatBotEnded.connect(function() {
            chatBotButton.isRunning = false
            chatBotButton.enabled = true
        })
    }
}
