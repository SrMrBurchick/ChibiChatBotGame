import QtQuick
import QtQuick.Layouts
import Base

Rectangle {
    id: root
    property bool isExpanded: false
    property bool isEnabled: true
    color: Style.settingsDelegateBGColor
    border.color: Style.settingsDelegateBorderColor
    border.width: 2
    width: parent.width
    height: isExpanded ? content.height + contentLoader.height + 5 : content.height
    RowLayout {
        id: content
        spacing: 10
        BaseText {
            id: description
            Layout.leftMargin: 5
            text: name
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
                    if (root.isEnabled) {
                        root.isExpanded = !root.isExpanded
                    }
                }
            }
        }
    }

    Loader {
        id: contentLoader
        visible: root.isExpanded
        anchors.top: content.bottom
        width: root.width
        sourceComponent: component
        onLoaded: {
            componentLoaded(component)
        }
    }

    signal componentLoaded(Component loaded_component)
}
