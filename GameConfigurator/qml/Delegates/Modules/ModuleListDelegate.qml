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

    color: Style.actionListItemBGColor
    property Module module: undefined
    border.color: module ? module.isSelected ? Style.actionListItemBorderSelectedColor : Style.actionListItemBorderColor : Style.actionListItemBorderColor
    border.width: 2
    width: parent.width - 5
    height: 50

    RowLayout {
        id: panel
        anchors.fill: root
        anchors.margins: 5

        BaseText {
            font.pixelSize : 24
            text: module ? module.name : ""
            Layout.alignment: Qt.AlignHCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (ModulesManager != undefined) {
                ModulesManager.selectModule(index);
            }
        }
    }

    Component.onCompleted: {
        if (ModulesManager != undefined) {
            module = ModulesManager.getModule(index);
        }
    }
}
