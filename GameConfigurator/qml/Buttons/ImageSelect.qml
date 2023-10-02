import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: image_select

    visible: true

    Button {
        text: qsTr("Choose Image...")
        onClicked: fileDialog.open()
    }
    //
    // Image {
    //     id: image
    //     anchors.fill: parent
    //     fillMode: Image.PreserveAspectFit
    // }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            // image.source = selectedFile
            console.log(selectedFile)

        }
    }
}
