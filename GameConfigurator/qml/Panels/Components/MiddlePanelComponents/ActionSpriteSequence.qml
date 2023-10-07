import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Delegates

Item {
    id: root
    anchors.fill: parent

    Rectangle {
        id: panel
        anchors.fill: root

        Text {
            id: title
            anchors.margins: 5
            anchors.horizontalCenter: panel.horizontalCenter
            text: "Action animation sequance"
        }

        DelegateModel {
            id: actionsModel
            model: ListModel {
                ListElement{
                    column: 3
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 4
                    row: 0
                }

                ListElement {
                    column: 2
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }

                ListElement {
                    column: 3
                    row: 1
                }
            }

            delegate: ActionSpriteDelegate {
                width: scrollView.height
                height: scrollView.height
            }
        }

        ScrollView {
            id: scrollView
            anchors.top: title.bottom
            height: panel.height - title.height
            width: panel.width
            clip: true

            ListView {
                width: parent.width
                spacing: 3
                orientation: ListView.Horizontal
                model: actionsModel
                cacheBuffer: 50
            }
        }
    }
}

