import QtQuick
import QtQuick.Layouts
import Base
import Delegates
import Panels
import Managers
import TwitchManagerComponent
import Buttons

BasePanel {
    id: root
    property ChannelPointsReward target
    readonly property int offset: 10
    height: description.height + contentLoader.height + 2 * offset
    // height: 400
    radius: 15.0

    BaseText {
        id: description
        anchors.horizontalCenter: parent.horizontalCenter
        text: name
        font.pixelSize: 32
        font.bold: true
    }

    Loader {
        id: contentLoader
        visible: root.isExpanded
        anchors.top: description.bottom
        width: root.width
        sourceComponent: component
        onLoaded: {
            componentLoaded(component)
        }

    }

    signal componentLoaded(Component loaded_component)
}
