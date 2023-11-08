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

        TextField {
            Layout.margins: 10
            Layout.fillWidth: true
            horizontalAlignment: TextInput.AlignHCenter
            validator: RegularExpressionValidator{regularExpression: /^[0-9\./]+$/}
            background: Rectangle {
                color: Style.propertyDelegateBorderColor
            }
            text: "0.0"
            font.pixelSize : 18
            color: Style.textColor
            onTextEdited: {
                chance = parseFloat(text)
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
