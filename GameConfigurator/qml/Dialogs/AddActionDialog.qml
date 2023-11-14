import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels

Dialog {
    id: root
    property string actionName
    property bool isCustomSelected: false

    title: "Add action"
    standardButtons: Dialog.Cancel | Dialog.Save
    readonly property var defaultActions: ["walk", "fall", "climb", "standby", "custom"]

    RowLayout {
        anchors.fill: root
        TextEdit {
            visible: isCustomSelected
            font.pointSize: 14

            text: actionName

            focus: true
            wrapMode: TextEdit.Wrap

            onEditingFinished: {
                actionName = text
            }
        }

        ComboBox {
            id: actionSelector
            visible: !isCustomSelected
            model: defaultActions

            onActivated: {
                if (currentText == "custom" && isCustomSelected == false) {
                    isCustomSelected = true
                } else {
                    actionName = currentText
                }
            }

            Component.onCompleted: {
                setupDialog()
            }
        }
    }

    onAccepted: {
        addNewAction(actionName);
        actionName = ""
        isCustomSelected = false
    }
    onRejected: {
        actionName = ""
        isCustomSelected = false
    }

    signal addNewAction(string newAction)

    Component.onCompleted: {
        var loadedActions = ActionsManager.actionsListModel.getActions()
        loadedActions.forEach(function(action) {
            if (defaultActions.includes(action) && actionSelector.model.includes(action)) {
                actionSelector.model = actionSelector.model.filter(function (item) {
                    return item != action;
                })
            }
        })

        actionName = actionSelector.currentText
    }

    function setupDialog() {
        if (actionSelector.currentText == "custom" && isCustomSelected == false) {
            root.isCustomSelected = true
        } else {
            root.actionName = actionSelector.currentText
        }
    }
}
