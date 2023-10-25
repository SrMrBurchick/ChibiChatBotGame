import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Base

Button {
    id: button
    highlighted: true
    onClicked: {
        console.log("Clicked! " + button.text)
    }

    contentItem: Text {
        text: button.text
        color: Style.textColor
        font.pixelSize: Style.buttonFontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    background: Rectangle {
        radius: Style.buttonRadiusSize
        color: button.hovered ? Style.buttonHoveredBGColor : Style.buttonBGColor
        border.color: Style.buttonBorderColor
        border.width: Style.buttonBorderSize
    }

    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter // Center the item in the layout
}
