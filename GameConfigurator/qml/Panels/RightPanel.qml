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

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Animation preview"
                }

                // TODO: Replace with AnimationSequance
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "green"
                    width: 200
                    height: 200
                }

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter

                    spacing: 4

                    Button {
                        text: "Play"
                    }

                    Button {
                        text: "Pause"
                    }

                }
            }
        }

        Button {
            Layout.fillWidth: true
            text: "Save"
            onClicked: saveActionsConfig()
        }

    }


    signal saveActionsConfig()
}

