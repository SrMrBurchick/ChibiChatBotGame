import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: button
    highlighted: true
    font.pixelSize: 32 // Set the font size
    onClicked: {
        console.log("Clicked! " + button.text)
    }

    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter // Center the item in the layout
}
