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

        Item {
            id: delegateManager
            property int currentIndex
        }

        delegate: ActionListDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            onRemoveElement: {
                ActionsManager.actionsListModel.removeElement(index)
            }
            onChangeElement: {
                ActionsManager.actionsListModel.changeElement(index, action)
            }
            onElementSelected: {
                delegateManager.currentIndex = index

                if (ActionsManager.sequenceModel != undefined) {
                    ActionsManager.sequenceModel.setActiveAction(action);
                }
            }
        }
    }

    function addAction(action) {
        console.log("Add new action: " + action)
        ActionsManager.actionsListModel.addNewAction(action)
    }
}

