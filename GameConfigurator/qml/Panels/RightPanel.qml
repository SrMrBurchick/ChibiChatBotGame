import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels

Item {
    id: root
    anchors.fill: parent
    anchors.leftMargin: 6

    ColumnLayout {
        id: panel
        anchors.fill: root

        Text {
            id: title
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Animation preview"
        }

        // TODO: Replace with AnimationSequance
        Rectangle {
            anchors.horizontalCenter: panel.horizontalCenter
            color: "green"
            width: 100
            height: 100
        }

        RowLayout {
            anchors.horizontalCenter: panel.horizontalCenter

            spacing: 4

            Button {
                text: "Play"
            }

            Button {
                text: "Pause"
            }

        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            Layout.fillWidth: true
            text: "Save"
            onClicked: saveActionsConfig()
        }

    }


    signal saveActionsConfig()
}

