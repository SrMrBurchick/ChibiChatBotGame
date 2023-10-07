import QtQuick

Rectangle {
    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2
    height: 80
    width: 80

    color: "yellow"

    Text {
        text: "(" + column + " : " + row + ")"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Clicked")
        }
    }
}
