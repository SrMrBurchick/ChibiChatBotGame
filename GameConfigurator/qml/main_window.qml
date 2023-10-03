import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Pages

ApplicationWindow {
    visible: true

    StackView {
        anchors.fill: parent
        initialItem: MainPage {}

    }
}
