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
            }
        }
    }

    function addAction(action) {
        console.log("Add new action: " + action)
        actionsModel.addNewAction(action)
    }
}

