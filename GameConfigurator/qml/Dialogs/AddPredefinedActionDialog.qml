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
    property var notSupportedActions: ["fall"]
    width: 300
    height: 300

    title: "Add action"
    standardButtons: Dialog.Cancel | Dialog.Save

    background: Rectangle {
        color: Style.pageBGColor
    }

    RowLayout {
        id: predefinedConfigurator
        anchors.fill: root
        spacing: 10

        ComboBox {
            id: actionSelector
            model: ActionsManager.actionsListModel.getActions()

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

    Label {
        id: configuredText
        anchors.fill: root
        visible: false
        BaseText {
            text: "All actions already configured"
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
        notSupportedActions.forEach(function(action) {
            if (actionSelector.model.includes(action)) {
                actionSelector.model = actionSelector.model.filter(function (item) {
                    return item != action;
                })
            }
        })
        actionName = actionSelector.currentText
    }

    function onActionAdded(action) {
        if (actionSelector.model.includes(action)) {
            actionSelector.model = actionSelector.model.filter(function (item) {
                return item != action;
            })
        }
    }

    function onActionRemoved(action) {
        if (!actionSelector.model.includes(action)) {
            var tempList = actionSelector.model
            tempList = [action].concat(tempList)

            actionSelector.model = tempList;
        }
    }

    function setupDialog(actions) {
        actions.forEach(function(action) {
            if (actionSelector.model.includes(action)) {
                actionSelector.model = actionSelector.model.filter(function (item) {
                    return item != action;
                })
            }
        })

        if (actionSelector.count == 0) {
            predefinedConfigurator.visible = false
            configuredText.visible = true
            actionName = ""
        } else {
            predefinedConfigurator.visible = true
            configuredText.visible = false
            actionName = actionSelector.currentText
        }
    }
}
