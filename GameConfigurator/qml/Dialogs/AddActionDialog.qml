import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import ActionsManagerComponent

Dialog {
    id: root
    property string actionName
    property bool isCustomSelected: false

    title: "Add action"
    standardButtons: Dialog.Cancel | Dialog.Save
    readonly property var defaultActions: ["custom"]

    RowLayout {
        anchors.fill: root
        TextField {
            width: parent.width
            focus: true
            wrapMode: TextEdit.Wrap
            visible: isCustomSelected
            font.pointSize: 14

            text: actionName

            onTextEdited: {
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

    function setupDialog() {
        actionSelector.model = ActionsManager.getPossibleActionsToAdd().concat(defaultActions)

        root.isCustomSelected = actionSelector.count == 1

        if (!isCustomSelected) {
            actionName = actionSelector.currentText
        }
    }
}
