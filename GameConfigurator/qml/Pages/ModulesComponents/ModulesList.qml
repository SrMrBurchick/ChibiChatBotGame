import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Panels
import ConfigComponent
import Base
import Managers
import Delegates

BasePanel {
    id: root

    ColumnLayout {
        anchors.fill: root
        spacing: 5

        BaseText {
            Layout.alignment: Qt.AlignHCenter
            text: "Modules List"
            font.pixelSize: 32
        }

        ListView {
            id: modulesList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ModulesManager.modulesCount
            delegate: ModuleListDelegate {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Component.onCompleted: {
    }
}
