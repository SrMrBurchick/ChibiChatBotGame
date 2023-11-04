import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    property string actionName
    property bool isCustomSelected: false

    title: "Add action"
    standardButtons: Dialog.Cancel | Dialog.Save

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
            model: ["walk", "fall", "climb", "standby", "custom"]

            onActivated: {
                if (currentText == "custom" && isCustomSelected == false) {
                    isCustomSelected = true
                } else {
                    actionName = currentText
                }
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
        actionName = actionSelector.currentText
    }
}
