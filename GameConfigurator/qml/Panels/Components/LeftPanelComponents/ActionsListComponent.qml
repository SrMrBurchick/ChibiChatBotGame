import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ActionsModels
import Delegates
import Panels
import ConfigComponent
import ActionsManagerComponent

ScrollView {
    anchors.margins: 4

    ListView {
        spacing: 5
        id: actionsList
        model: ActionsListModel {}

        delegate: ActionListDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            onRemoveElement: (index) => {
                // ActionsManager.actionsListModel.removeElement(index)
            }
            onChangeElement:(index, action) => {
                // ActionsManager.actionsListModel.changeElement(index, action)
            }
            onElementSelected:(index) => {
                // ActionsManager.actionsListModel.setSelectedActionIndex(index)
            }
        }
    }

    function addAction(action) {
        var count = actionsList.count
        console.log("Add new action: ", action, "Count:", count)
        ActionsManager.addNewAction(action)
        // ActionsManager.actionsListModel.addNewAction(action)

        if (count == 0) {
            // ActionsManager.actionsListModel.setDefaultSelected()
        }
    }

    Component.onCompleted: {
        actionsList.model.subscribeOnTarget(ActionsManager)
    }
}
