import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Buttons
import Delegates
import Panels
import ConfigComponent
import Dialogs
import Base
import ConfigTools
import Managers
import Buttons

Item {
    id: root
    anchors.fill: parent

    property StackView rootStack: StackView.view

    ColumnLayout {
        anchors.fill: root
        anchors.margins: 10
        spacing: 10

        // Modules panel
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ModulesList {
                id: modulesList
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: 400
            }

            ModulePreviewPanel {
                id: previewPanel
                Layout.fillWidth: true
                Layout.fillHeight: true
                // Layout.preferredHeight: modulesList.height
            }
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: 400

                BaseButton {
                    text: "Refresh"
                    font.pointSize: 14
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Layout.margins: 10

                    onClicked: {
                        if (ModulesManager != undefined) {
                            ModulesManager.initModules()
                        }
                    }
                }

                BaseButton {
                    text: "Back"
                    font.pointSize: 14
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Layout.bottomMargin: 10
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10

                    onClicked: {
                        rootStack.pop()
                    }
                }

            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                BaseButton {
                    text: "Save"
                    font.pointSize: 14
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Layout.bottomMargin: 10
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10

                    onClicked: {
                        // rootStack.pop()
                    }
                }
            }

        }
    }

    Component.onCompleted: {
        if (ModulesManager != undefined) {
            ModulesManager.initModules()
        }
    }
}
