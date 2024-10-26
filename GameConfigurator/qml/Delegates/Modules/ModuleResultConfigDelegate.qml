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
    property ModuleBindResultConfig resultConfig: undefined

    // signal componentLoaded(Component loaded_component)
    Component.onCompleted: {
        if (moduleBindConfig != undefined) {
            resultConfig = moduleBindConfig.getResultConfig(index)
        }
    }
}
