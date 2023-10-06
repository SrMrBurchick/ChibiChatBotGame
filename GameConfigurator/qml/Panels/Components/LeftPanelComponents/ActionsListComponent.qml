import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GameActions
import Delegates

ScrollView {
    anchors.fill: parent
    anchors.margins: 4

    ListView {
        spacing: 5
        model: ActionsListModel {
            id: actions_model
        }

        delegate: ActionListDelegate {
            width: parent.width
            height: 30
            onRemoveElement: {
                actions_model.removeElement(index)
            }
        }
    }

    signal addAction()
}

