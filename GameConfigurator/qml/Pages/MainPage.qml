import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Managers
import Delegates
import Base

Item {
    id: menu
    anchors.fill: parent
    anchors.centerIn: parent

    signal openConfig()


    GameRunningDelegate {
        id: gameRunningDelegate
        anchors.top: menu.top
    }

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
                GlobalConfig.isBusy = true
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
                GlobalConfig.isBusy = true
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
        gameRunningDelegate.close()
        ProcessManager.onGameEnded.connect(function() {
            gameRunningDelegate.close()
            gameButton.isRunning = false
            gameButton.enabled = true
            GlobalConfig.isBusy = false
        })
        ProcessManager.onGameRunningAt.connect(function(gameInfo) {
            GlobalConfig.isBusy = false
            gameButton.isRunning = true
            gameButton.enabled = true

            gameRunningDelegate.gameInfo = gameInfo
            gameRunningDelegate.show()
        })

        ProcessManager.onChatBotStarted.connect(function() {
            chatBotButton.isRunning = true
            chatBotButton.enabled = true
            GlobalConfig.isBusy = false
        })
        ProcessManager.onChatBotEnded.connect(function() {
            chatBotButton.isRunning = false
            chatBotButton.enabled = true
            GlobalConfig.isBusy = false
        })
    }
}
