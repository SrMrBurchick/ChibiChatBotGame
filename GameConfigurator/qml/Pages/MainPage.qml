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
            id: botButton
            scaler: 1.5
            property bool isRunning: false
            text: isRunning ? "Stop twitch bot" : "Start twitch bot"
            onClicked: {
                botButton.enabled = false
                GlobalConfig.isBusy = true
                if (ProcessManager.isBotRunning()) {
                    ProcessManager.stopBotRunning();
                } else {
                    ProcessManager.runBot()
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

        ProcessManager.onBotStarted.connect(function() {
            botButton.isRunning = true
            botButton.enabled = true
            GlobalConfig.isBusy = false
        })
        ProcessManager.onBotEnded.connect(function() {
            botButton.isRunning = false
            botButton.enabled = true
            GlobalConfig.isBusy = false
        })
    }
}
