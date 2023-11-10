import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs

Rectangle {
    id: root
    color: Style.actionListItemBGColor
    width: parent.width - 5
    height: 50

    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

        BaseText {
            id: actionName
            font.pixelSize : 24
            text: name
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
        }

        SpinBox {
            id: spinbox
            from: 0
            to: 100
            value: chance
            editable: true
            contentItem: TextInput {
                text: spinbox.textFromValue(spinbox.value, spinbox.locale)
                font: spinbox.font
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !spinbox.editable
                validator: spinbox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onTextChanged: {
                    spinbox.value = parseInt(text);
                }
            }
            onValueModified: {
                chance = spinbox.value
            }
        }

        Rectangle {
            width: 20
            height: 20
            color: "transparent"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: Style.trashCanImage
                }
                onClicked: {
                    removeElement(index);
                }
            }
        }
    }

    signal removeElement(int index)
    signal changeChance(int index, real chance)
}
