import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import GameActions

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: panel
        anchors.fill: root

        Text {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Action animation sequence"
        }

        DelegateModel {
            id: actionsDelegateModel
            model: AnimationSequenceModel {
                id: actionsModel
            }

            delegate: ActionSpriteDelegate {
                width: scrollView.height
                height: scrollView.height
                onPlaceItemAtPosition: (oldIndex, newIndex) => {
                    actionsModel.placeItemAt(oldIndex, newIndex);
                }
            }
        }

        ScrollView {
            id: scrollView
            anchors.top: title.bottom
            height: panel.height - title.height
            width: panel.width
            clip: true

            ListView {
                width: parent.width
                spacing: 3
                orientation: ListView.Horizontal
                model: actionsDelegateModel
                cacheBuffer: 50
            }
        }
    }
}

