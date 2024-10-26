import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Panels
import Managers
import Base
import Delegates
import ActionsManagerComponent

Dialog {
    id: root
    title: "Select action"
    standardButtons: Dialog.Cancel | Dialog.Ok
    property string action : ""

    width: 400
    background: Rectangle {
        color: Style.pageBGColor
    }

    contentItem: RowLayout {
        anchors.fill: root
        PropertyComboBoxDelegate {
            Layout.fillWidth: true
            Layout.margins: 10

            fieldName: "Bind:"
            targetModel: ActionsManager.getActionsName()

            onValueChanged:(value) => {
                console.log("Selected: ", value)
                action = value
            }
        }
    }

    onAccepted: {
        actionSelected(action)
    }

    signal actionSelected(string action)
}
