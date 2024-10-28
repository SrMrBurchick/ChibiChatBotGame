import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Base
import Dialogs
import Panels
import Managers
import ModuleSettings
import Buttons

Rectangle {
    id: root

    property ModuleBindResultPool poolConfig: undefined
    property ModuleBindResult result: undefined
    // color: "transparent"

    // border.color: Style.settingsDelegateBorderColor
    color: Style.actionListItemBGColor
    radius: 5

    Drag.active: dragArea.drag.active
    anchors.margins: 20

    height: content.height

    RowLayout {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 5

        BaseText {
            text: "# " + (index + 1)
            font.pixelSize: 28
            Layout.alignment: Qt.AlignVCenter
        }

        BaseText {
            text: result.targetActionName
            font.pixelSize: 28
            Layout.alignment: Qt.AlignVCenter
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
        }

        RemoveButton {
            id: button
            height: 30
            width: 30
            Layout.alignment: Qt.AlignVCenter
            onClicked: {
                if (poolConfig != undefined) {
                    poolConfig.removeResult(index)
                }
            }
        }
    }

    MouseArea {
        id: dragArea
        anchors.fill: root
        anchors.rightMargin: button.width

        drag.axis: Drag.YAxis
        drag.target: root

        onReleased: {
            if (poolConfig != undefined) {
                poolConfig.commitSwap(index)
            }
        }

        onDoubleClicked: {
        }
    }

    DropArea {
        anchors.fill: root
        onEntered:(drag) => {
            console.log("Entered area: ",dragArea.DelegateModel.itemsIndex)
            // if (dragArea.DelegateModel.itemsIndex >= 0) {
                poolConfig.setNewPosition(dragArea.DelegateModel.itemsIndex)
            // }
        }
    }

    Component.onCompleted: {
        console.log("Create module pool result")
        if (poolConfig != undefined) {
            result = poolConfig.getResult(index)
        }
    }
}
