import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import ConfigComponent

Rectangle {
    id: root

    width: parent.width
    height: 40
    color: Style.gameRunningInfoBGColor
    property string gameInfo: ""

    // Opacity property for the fade-in and fade-out effect
    property real opacityValue: 0

    opacity: opacityValue

    // Add a Behavior to animate the opacity change
    Behavior on opacityValue {
        NumberAnimation {
            duration: 1000
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: 500
            onRunningChanged: {
                if (!running && root.color != Style.gameRunningInfoBGColor) {
                    root.color = Style.gameRunningInfoBGColor
                    console.log("Set default color")
                }
            }
        }
    }

    RowLayout {
        id: content
        anchors.fill: root
        Text {
            id: textGameInfo
            anchors.centerIn: parent
            color: Style.gameRunningInfoTextColor
            text: "Game running at: " + root.gameInfo
            font.pixelSize: 24
        }

        Rectangle {
            anchors.left: textGameInfo.right
            anchors.leftMargin: 10
            height: content.height - 10
            width: height
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.copyImage
                }
                onClicked: {
                    Config.saveDataToClipboard(root.gameInfo)
                    root.color = "#C2B93E"
                }
            }
        }
    }

    function show() {
        root.opacityValue = 1
    }

    function close() {
        root.opacityValue = 0
    }
}

