import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates
import ActionsModels
import Panels
import ConfigComponent
import Base
import Managers

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 10

    BasePanel {
        id: panel
        anchors.fill: root
        anchors.margins: 0

        BaseText {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Action animation sequence"
            font.pixelSize : 32
        }

        DelegateModel {
            id: actionsDelegateModel
            model: AnimationSequenceModel {

            }

            delegate: ActionSpriteDelegate {
                width: scrollView.height
                height: scrollView.height
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

    function clearModel() {
        // ActionsManager.sequenceModel.clearModel();
    }

    Component.onCompleted: {
        actionsDelegateModel.model.subscribeOnTarget(ActionsManager)
    }
}
