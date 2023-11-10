import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GameActions
import Delegates
import Panels
import ConfigComponent

ScrollView {
    anchors.margins: 4

    ListView {
        spacing: 5
        id: actionsList
        model: ActionsManager.actionsListModel

        delegate: ActionListDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            onRemoveElement: {
                ActionsManager.actionsListModel.removeElement(index)
            }
            onChangeElement: {
                ActionsManager.actionsListModel.changeElement(index, action)
            }
            onElementSelected: {
                ActionsManager.actionsListModel.setSelectedActionIndex(index)
            }
        }
    }

    function addAction(action) {
        console.log("Add new action: " + action)
        ActionsManager.actionsListModel.addNewAction(action)
    }
}

