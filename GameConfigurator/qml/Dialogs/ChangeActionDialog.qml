import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    property string actionName
    title: "Change action"
    standardButtons: Dialog.Cancel | Dialog.Save

    Label {
        anchors.fill: root
        TextEdit {
            width: root.width
            font.pointSize: 14

            text: actionName

            focus: true
            wrapMode: TextEdit.Wrap

            onEditingFinished: {
                actionName = text
            }
        }
    }

    onAccepted: {
        console.log("Saved: " + actionName);
        changeAction(actionName);
    }
    onRejected: console.log("Cancel clicked")

    signal changeAction(string action)
}
