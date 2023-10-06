import QtQuick
import QtQuick.Layouts
import Base

Rectangle {
    id: root

    border.color: "black"
    border.width: 2

    RowLayout {
        anchors.margins: 4
        anchors.fill: root

        Text {
            text: name
        }

        Rectangle {
            Layout.fillWidth: true
        }

        Rectangle {
            width: 20
            height: 20

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.trashCanImage
                }
                onClicked: {
                    removeElement(index.send);
                }
            }
        }
    }

    signal removeElement(int index);
}
