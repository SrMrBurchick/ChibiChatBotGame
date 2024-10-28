import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Panels
import Managers
import ModuleSettings
import Buttons
import Delegates

Rectangle {
    id: root

    property ModuleBindResultPool poolConfig: undefined
    property ModuleBindResultConfig resultConfig: undefined

    color: "transparent"
    height: content.height
    border.width: 5
    border.color: Style.propertyDelegateBorderColor
    radius: 5

    BindModuleAction {
        id: bindActionDialog
        onActionSelected: (action) => {
            poolConfig.bindNewAction(ActionsManager, ModulesManager, action)
        }
    }

    ColumnLayout {
        id: content
        spacing: 5
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 10

        BaseText {
            text: resultConfig.targetOutput.name + " :: " + poolConfig.postfix
            font.pixelSize: 32
            font.bold: true
        }

        DelegateModel {
            id: actionsDelegateModel
            model: root.poolConfig.resultsCount
            delegate: ModuleResultDelegate {
                poolConfig: root.poolConfig
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.margins: 10
            visible: poolConfig ? poolConfig.resultsCount > 0 : false
            height: 100
            color: Style.panelBGColor
            radius: 15

            ScrollView {
                id: resultsList
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                ListView {
                    spacing: 10
                    anchors.fill: parent
                    orientation: ListView.Vertical
                    model: actionsDelegateModel
                    cacheBuffer: 50
                }
            }
        }

        BaseButton {
            text: "Bind new action"
            font.pointSize: 14
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.margins: 10
            visible: result ? true : false

            onClicked: {
                bindActionDialog.open()
            }
        }

    }

    Component.onCompleted: {
        console.log("Create module pool config")
        if (root.resultConfig != undefined) {
            poolConfig = root.resultConfig.getPool(index)
        }
    }
}
