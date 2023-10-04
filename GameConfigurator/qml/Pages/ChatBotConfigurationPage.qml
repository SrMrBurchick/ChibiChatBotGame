import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons

// TODO: Refactor style
Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view

    // Title
    Text {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        text: "ChatBot configuration"
        font.pointSize: 24
    }


    // Panel
    ColumnLayout {
        id: panel
        width: root.width
        anchors.top: title.bottom
        anchors.bottom: root.bottom

        // Settings
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 10

            // TODO: Move model to c++
            model: ListModel {
                ListElement {
                    name: "Twitch channel name"
                    value: "IcePepePopusk"
                }

                ListElement {
                    name: "GameClient port"
                    value: "12345"
                }
            }

            delegate: Rectangle {
                width: panel.width
                height: delegateContent.height

                RowLayout {
                    id: delegateContent
                    Text {
                        text: name + ": "
                        font.pointSize: 16
                    }
                    Rectangle {
                        clip: true
                        border.color: "black"
                        border.width: 2
                        color: "gray"
                        Layout.minimumHeight: 20
                        Layout.minimumWidth: 40

                        TextEdit {

                            anchors.verticalCenter: parent.verticalCenter
                            id: edittibleValue
                            text: value
                        }
                    }
                }
            }
        }

        // Controls
        RowLayout {
            Layout.fillWidth: true

            BaseButton {
                text: "Back"
                onClicked: {
                    rootStack.pop()
                }
            }

            Rectangle {
                Layout.fillWidth: true
            }

            BaseButton {
                text: "Save"
                onClicked: saveChatBotConfig()
            }

        }

    }

    signal saveChatBotConfig()
}

