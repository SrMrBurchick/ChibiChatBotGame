import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ScrollView {
    anchors.fill: parent
    anchors.margins: 4

    ListView {
        // TODO: Add c++ model
        model: ListModel {
            ListElement {
                name: "Walk"
            }
            ListElement {
                name: "Climb"
            }
            ListElement {
                name: "Fall"
            }
            ListElement {
                name: "Dance"
            }
        }

        delegate: Rectangle {
            width: parent.width
            height: 30

            Text {
                text: parent.width + "x" + parent.height + "->" + name
            }
        }
    }

    signal addAction()
}

