import QtQuick

Rectangle {
    id: root
    property int column
    property int row
    property string imageSource
    property int frameSize

    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    color: "yellow"

    Text {
        text: "(" + column + " : " + row + ")"
    }
    //
    // Sprite {
    //     // anchors.fill: root
    //     frameWidth: frameSize
    //     frameHeight: frameSize
    //     source: imageSource
    // }
}
