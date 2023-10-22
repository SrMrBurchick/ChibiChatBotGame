import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SystemTools

Rectangle {
    id: root
    width: title.width + body.width + 50
    height: title.height + body.height + 50
    color: "lightblue"
    radius: 10
    border.color: "black"
    border.width: 1

    property string description: ""
    property string message: ""
    property NotificationListModel rootModel: NotificationListModel{}

    // Position the notification in the top right corner
    anchors.margins: 10

    // Opacity property for the fade-in and fade-out effect
    property real opacityValue: 0

    // Add a Behavior to animate the opacity change
    Behavior on opacityValue {
        NumberAnimation {
            duration: 500 // Adjust the duration as needed
            onRunningChanged: {
                if (!running && opacityValue == 0) {
                    rootModel.removeNotification(index)
                }
            }
        }
    }

    // Use the opacity property for the fade-in and fade-out effect
    opacity: opacityValue

    Text {
        id: title
        anchors.top: root.top
        anchors.left: root.left
        anchors.margins: 10
        font.pixelSize: 18 // Set the font size
        font.bold: true
        text: description
    }
    Text {
        id: body
        anchors.top: title.bottom
        anchors.left: root.left
        anchors.margins: 10
        font.pixelSize: 14 // Set the font size
        font.italic: true
        text: message
    }

    // Add a function to trigger the animation
    function show() {
        opacityValue = 1;
    }

    // Add a function to trigger the fade-out animation
    function hide() {
        opacityValue = 0;
    }

    // Trigger the fade-out animation after a delay
    Timer {
        id: hideTimer
        running: opacityValue == 1
        interval: 3000 // Adjust the delay as needed (milliseconds)
        onTriggered: hide()
    }

    // Trigger the fade-out animation when the notification is clicked
    MouseArea {
        enabled: opacityValue == 1
        anchors.fill: parent
        onClicked: {
            hide()
        }
        onEntered: {
            hideTimer.restart()
        }
    }

    Component.onCompleted: {
        show()
    }
}
