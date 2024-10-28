import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Panels
import Managers
import ModuleSettings
import Delegates
import Buttons

Rectangle {
    id: root

    property ModuleBindConfig moduleBindConfig: undefined

    color: Style.settingsDelegateBGColor
    height: panel.height

    border.width: 5
    border.color: Style.propertyDelegateBorderColor
    radius: 20

    ColumnLayout {
        id: panel
        spacing: 10
        width: root.width

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.margins: 10
            spacing: 10

            BaseText {
                text: moduleBindConfig ? moduleBindConfig.targetAction.name : ""
                font.pixelSize: 32
                font.bold: true
            }

            RemoveButton {
                height: 40
                width: 40
                Layout.alignment: Qt.AlignVCenter
                onClicked: {
                    console.log("Remove clicked");
                    if (ModulesManager != undefined) {
                        var module = ModulesManager.getSelectedModule();
                        module.removeBindConfig(index)
                    }
                }
            }
        }

        Column {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            spacing: 10
            Repeater {
                anchors.fill: parent
                model: root.moduleBindConfig.resultsConfigCount
                delegate: ModuleResultConfigDelegate {
                    // anchors.horizontalCenter: parent.horizontalCenter
                    // width: parent.width / 2.0
                    anchors.left: parent.left
                    anchors.right: parent.right
                    moduleBindConfig: root.moduleBindConfig
                }
            }
        }

    }

    Component.onCompleted: {
        console.log("Create module bind")
        if (ModulesManager != undefined) {
            var module = ModulesManager.getSelectedModule();
            moduleBindConfig = module.getBindConfig(index)
        }
    }
}
