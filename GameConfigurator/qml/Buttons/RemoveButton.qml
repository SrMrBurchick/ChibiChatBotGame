import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Base

Rectangle {
    id: root

    color: "transparent"
    property bool hovered: false

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        Image {
            anchors.fill: parent
            source: root.hovered ? Style.trashCanHoveredImage : Style.trashCanImage
        }
        onEntered: {
            console.log("onEntered")
            root.hovered = true
        }
        onExited: {
            console.log("onExited")
            root.hovered = false
        }
        onClicked: {
            root.clicked()
        }
    }

    signal clicked()
}
