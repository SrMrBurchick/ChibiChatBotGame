import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import Delegates
import Base

Dialog {
    id: root
    property string actionName
    property real chance

    title: "Add action"
    standardButtons: Dialog.Cancel | Dialog.Save

    background: Rectangle {
        color: Style.pageBGColor
    }

    RowLayout {
        anchors.fill: root
        spacing: 10

        ComboBox {
            id: actionSelector
            model: ActionsManager.actionsListModel

            onActivated: {
                actionName = currentText
            }
        }

        SpinBox {
            id: spinbox
            from: 0
            to: 100
            editable: true
            contentItem: TextInput {
                text: spinbox.textFromValue(spinbox.value, spinbox.locale)
                font: spinbox.font
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !spinbox.editable
                validator: spinbox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onTextChanged: {
                   root.chance = parseInt(text);
                }
            }
        }



    }

    onAccepted: {
        addNewAction(actionName, chance);
        actionName = ""
    }
    onRejected: {
        actionName = ""
    }

    signal addNewAction(string newAction, real chance)

    Component.onCompleted: {
        actionName = actionSelector.currentText
    }
}
