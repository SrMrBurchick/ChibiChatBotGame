import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Buttons
import Panels
import Dialogs
import ConfigComponent
import Managers
import Base

Item {
    id: root
    anchors.fill: parent
    property StackView rootStack: StackView.view

    BaseButton {
        text: "Back"
        Layout.fillWidth: true
        Layout.maximumWidth: 400
        onClicked: {
            rootStack.pop()
        }
    }
}

