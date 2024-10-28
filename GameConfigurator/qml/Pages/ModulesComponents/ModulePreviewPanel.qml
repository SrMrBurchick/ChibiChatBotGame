import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import Base
import Managers
import Delegates
import ModuleSettings
import Dialogs
import ActionsManagerComponent

BasePanel {
    id: root

    property Module module: undefined

    BindModuleAction {
        id: bindActionDialog
        onActionSelected: (action) => {
            if (module != undefined) {
                module.bindNewAction(ActionsManager, action)
            }
        }
    }

    ColumnLayout {
        anchors.fill: root
        spacing: 5

        BaseText {
            font.pixelSize : 32
            text: "Module " + (module ? module.name : "") + " preview"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 50
            PropertyDelegate {
                Layout.fillWidth: true
                Layout.margins: 10

                isEditable: false
                fieldName: "Name:"
                defaultText: module ? module.name : ""
            }

            PropertyDelegate {
                Layout.fillWidth: true
                Layout.margins: 10

                isEditable: false
                fieldName: "Version:"
                defaultText: module ? module.version : ""
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            BaseText {
                font.pixelSize : 24
                text: "Module bindings"
                Layout.alignment: Qt.AlignHCenter
            }

            BasePanel {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ScrollView {
                    anchors.fill: parent
                    clip: true

                    ListView {
                        anchors.fill: parent
                        spacing: 20
                        model: module.bindsCount
                        delegate: ModuleBindListDelegate {
                            anchors.left: parent.left
                            anchors.right: parent.right
                        }
                    }
                }
            }
        }

        BaseButton {
            text: "Bind new action"
            font.pointSize: 14
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.margins: 10
            visible: module ? true : false

            onClicked: {
                bindActionDialog.open()
            }
        }
    }

    Connections {
        target: ModulesManager
        onModuleSelected: {
            root.module = ModulesManager.getSelectedModule();

            console.log("New module selected =", root.module)
        }
    }

    Component.onCompleted: {
        root.module = ModulesManager.getSelectedModule();
    }
}
