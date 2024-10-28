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

    property ModuleBindConfig moduleBindConfig: undefined
    property ModuleBindResultConfig resultConfig: undefined
    // color: "transparent"

    color: Style.actionListItemBGColor
    border.width: 5
    border.color: Style.propertyDelegateBorderColor
    radius: 5

    height: content.height

    ColumnLayout {
        id: content
        width: root.width

        spacing: 5
        BaseText {
            font.pixelSize: 32
            font.bold: true
            text: resultConfig.targetOutput.name
            Layout.alignment: Qt.AlignHCenter
        }
        Column {
            id: poolsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            spacing: 10

            Repeater {
                model: root.resultConfig.poolsCount
                delegate: ModuleBindPoolDelegate {
                    // width: parent.width / 2.0
                    // anchors.horizontalCenter: parent.horizontalCenter
                    resultConfig: root.resultConfig
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("Create module result config")
        if (moduleBindConfig != undefined) {
            resultConfig = moduleBindConfig.getResultConfig(index)
        }
    }
}
