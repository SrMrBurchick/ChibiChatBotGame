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
        model: ActionsListModel {
            id: actionsModel
        }

        Item {
            id: delegateManager
            property int currentIndex
        }

        delegate: ActionListDelegate {
            actionIndex: index
            width: parent.width
            height: 30
            onRemoveElement: {
                actionsModel.removeElement(index)
            }
            onChangeElement: {
                actionsModel.changeElement(index, action)
            }
            onElementSelected: {
                delegateManager.currentIndex = index

                if (ActionsManager.sequenceModel != undefined) {
                    ActionsManager.sequenceModel.setActiveAction(action);
                }
            }
        }
    }

    Component.onCompleted: {
        if (Config.isConfigLoaded()) {
            Config.initActionsListModel(actionsModel)
        }

        ActionsManager.sequenceModel = actionsModel
    }

    function addAction(action) {
        console.log("Add new action: " + action)
        actionsModel.addNewAction(action)
    }
}

