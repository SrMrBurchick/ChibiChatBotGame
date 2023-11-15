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

    title: "Add action"
    height: 150
    standardButtons: Dialog.Cancel | Dialog.Save

    background: Rectangle {
        color: Style.pageBGColor
    }

    contentItem: RowLayout {
        anchors.fill: root
        RowLayout {
            id: predefinedConfigurator
            spacing: 10
            visible: false

            ComboBox {
                id: actionSelector
                Layout.fillWidth: true
                model: ActionsManager.actionsListModel.getActions()

                onActivated: {
                    actionName = currentText
                }
            }

            SpinBox {
                Layout.margins: 10
                id: spinbox
                from: 1
                to: 100
                value: chance
                editable: true
                stepSize: 1

                onValueModified: {
                    root.chance = spinbox.value
                }
            }
        }

        BaseText {
            id: configuredText
            visible: false

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
            configuredText.visible = false
            predefinedConfigurator.visible = true
            actionName = actionSelector.currentText
            chance = spinbox.value
        }
    }
}
