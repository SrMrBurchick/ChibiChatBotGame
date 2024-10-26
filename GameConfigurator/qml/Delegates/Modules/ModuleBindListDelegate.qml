import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Panels
import Managers
import ModuleSettings

Rectangle {
    id: root

    property ModuleBindConfig moduleBindConfig: undefined
    property bool isExpanded: false
    property bool isEnabled: true

    color: Style.actionListItemBGColor

    width: parent.width
    height: isExpanded ? panel.height + contentLoader.height + 5 : panel.height

    RowLayout {
        id: panel
        spacing: 10

        BaseText {
            text: moduleBindConfig ? moduleBindConfig.targetAction.name : ""
            Layout.leftMargin: 5
            font.pixelSize: 32
            font.bold: true
        }

        BaseText {
            text: root.isExpanded ? "▼" : "►"
            font.pixelSize: 32
            font.bold: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.isExpanded = !root.isExpanded
                }
            }
        }

    }

    // MouseArea {
    //     anchors.fill: parent
    //     onClicked: {
    //         if (ModulesManager != undefined) {
    //             var module = ModulesManager.getSelectedModule();
    //             module.selectBind(index)
    //         }
    //     }
    // }

    Loader {
        id: contentLoader
        visible: root.isExpanded
        anchors.top: panel.bottom
        width: root.width
        sourceComponent: Rectangle {

        }
        // onLoaded: {
        //     componentLoaded(component)
        // }
    }

    // signal componentLoaded(Component loaded_component)
    Component.onCompleted: {
        if (ModulesManager != undefined) {
            var module = ModulesManager.getSelectedModule();
            moduleBindConfig = module.getBindConfig(index)
        }
    }
}
