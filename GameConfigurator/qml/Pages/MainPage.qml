import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons

Page {
    id: menu
    anchors.fill: parent
    anchors.centerIn: parent


    ColumnLayout {
        anchors.centerIn: parent
        spacing: 24

        BaseButton {
            text: "Start game"
        }

        BaseButton {
            text: "Start chat bot"
        }

        BaseButton {
            text: "Configure"
        }

        BaseButton {
            text: "Exit"
            onClicked: {
                Qt.quit()
            }
        }
    }
}

