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
            // anchors.horizontalCenter: parent.horizontalCenter
            onRemoveElement: (index) => {
                ActionsManager.removeActionById(index);
            }
            onChangeElement:(index, action) => {
                ActionsManager.changeActionName(index, action);
            }
            onElementSelected:(index) => {
                ActionsManager.markSelectedAction(index)
            }
        }
    }

    function addAction(action) {
        ActionsManager.addNewAction(action)

        if (count == 0) {
            // ActionsManager.actionsListModel.setDefaultSelected()
        }
    }

    Component.onCompleted: {
        actionsList.model.subscribeOnTarget(ActionsManager)
    }
}
