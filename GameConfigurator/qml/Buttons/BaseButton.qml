import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Base

Button {
    id: root
    highlighted: true
    property real scaler: 1.0

    onClicked: {
        console.log("Clicked! " + button.text)
    }

    contentItem: Text {
        text: root.text
        color: Style.textColor
        font.pixelSize: Style.buttonFontSize * scaler
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    background: Rectangle {
        radius: Style.buttonRadiusSize
        color: root.hovered ? Style.buttonHoveredBGColor : Style.buttonBGColor
        border.color: Style.buttonBorderColor
        border.width: Style.buttonBorderSize
    }

    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter // Center the item in the layout
    //
    Layout.preferredWidth: root.implicitWidth * scaler
    Layout.preferredHeight: root.implicitHeight * scaler
}
