import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property string imageSource
    property int frameSize
    property int oldIndex
    property int newIndex

    // TODO: Move width and height to c++
    border.color: "black"
    border.width: 2

    Drag.active: dragArea.drag.active

    Text {
        text: "(" + column + " : " + row + ")"
    }

    MouseArea {
        id: dragArea
        anchors.fill: root

        drag.axis: Drag.XAxis
        drag.target: root

        onReleased: {
            console.log("Place item to location", root.oldIndex, root.newIndex)
        }
    }


    DropArea {
        anchors.fill: root
        onEntered:(drag) => {
            console.log("Enter to drop area", index)
            // root.tryToPlaceItem(drag.source.DelegateModel.itemsIndex, dragArea.DelegateModel.itemsIndex)
            console.log("Tobi pizda", drag.source.DelegateModel.itemsIndex, dragArea.DelegateModel.itemsIndex)
            console.log("Traget", drag.target)
        }
    }

    signal placeItemAtPosition(int oldIndex, int newIndex)

    function tryToPlaceItem(oldIndex, newIndex) {
        root.oldIndex = oldIndex
        root.newIndex = newIndex
    }
}
