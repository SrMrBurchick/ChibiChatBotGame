import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Base

Dialog {
    id: root
    title: "Help!"
    standardButtons: Dialog.Ok
    height: 300

    property Component customDelegate: ItemDelegate {
        text: "Item " + (index + 1)

        required property int index
    }

    property ListModel customModel: ListModel {
        Component.onCompleted: {
            for (var i = 0; i < 500; i++) {
                append({text: "Item " + (i + 1)});
            }
        }
    }

    background: Rectangle {
        color: Style.pageBGColor
    }

    contentItem: ScrollView {
        id: scrollView
        ScrollBar.horizontal.interactive: true
        ScrollBar.vertical.interactive: true
        clip: true

        ListView {
            id: listView
            model: customModel
            delegate: customDelegate
        }

    }

    onOpened: {
        var maxWidth = 0;
        for(var child in listView.contentItem.children) {
            var childWidth = listView.contentItem.children[child].width;
            if (maxWidth < childWidth) {
                maxWidth = childWidth;
            }
        }

        root.width = maxWidth
    }

    onAccepted: {
    }
}
