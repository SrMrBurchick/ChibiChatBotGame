import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import GameActions
import Panels
import ConfigComponent
import Base

Item {
    id: root
    anchors.fill: parent

    BasePanel {
        id: panel
        anchors.fill: root
        anchors.margins: 0

        BaseText {
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
                onRemoveItem: (index) => {
                    actionsModel.removeElement(index)
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

    Component.onCompleted: {
        if (Config.isConfigLoaded()) {
            Config.initAnimationsSequenceModel(actionsModel)
        }

        ActionsManager.sequenceModel = actionsModel
    }

    function clearModel() {
        console.log("Clear sequence")
        actionsModel.clearModel();
    }
}

