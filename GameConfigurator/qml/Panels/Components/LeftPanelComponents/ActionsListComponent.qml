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
            onRemoveElement: (index) => {
                ActionsManager.actionsListModel.removeElement(index)
            }
            onChangeElement:(index, action) => {
                ActionsManager.actionsListModel.changeElement(index, action)
            }
            onElementSelected:(index) => {
                ActionsManager.actionsListModel.setSelectedActionIndex(index)
            }
        }
    }

    function addAction(action) {
        var count = actionsList.count
        console.log("Add new action: ", action, "Count:", count)
        ActionsManager.actionsListModel.addNewAction(action)

        if (count == 0) {
            ActionsManager.actionsListModel.setDefaultSelected()
        }
    }
}
