import QtQml 2.0
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.12

import Controller 1.0

Window {
    id: window
    title: "Joystick Utility"
    width: 800
    height: 600
    minimumWidth: 550
    maximumWidth: 800
    maximumHeight: 600
    visible: true

    Controller {
        id: controller
        onJoystickConnected: {
            appMouseArea.enabled = true;
            saveButton.enabled = true;
            settingsPanel.opacity = 1.0;
        }
        onJoystickDisconnected: {
            appMouseArea.enabled = false;
            saveButton.enabled = false;
            settingsPanel.opacity = 0.5;
            settingsPanel.currentFocus = -1;
            settingsPanel.forceActiveFocus();
        }
        onGamepadKeyPressed: {
            console.log("Получена строка: ", buttonName);
            var item = buttonCommandSettings.itemAt(controller.focusedElementIndex);
            var textField = item.children[1];
            textField.text = buttonName;
            textField.focus = false;
        }
        onCommandMappingChanged: {
            var item;
            if (type == "button") {
                item = buttonCommandSettings.itemAt(index);
            } else if (type == "axis") {
                item = axisCommandSettings.itemAt(index);
            }

            var textField = item.children[1];
            textField.text = buttonName;
        }
        onGamepadAxisChanged: {
            console.log("Получена строка: ", axisName);
            console.log(controller.focusedElementIndex);
            var item = axisCommandSettings.itemAt(controller.focusedElementIndex);
            var textField = item.children[1];
            textField.text = axisName;
            textField.focus = false;
        }
        onSettingsUpdated: {
            settingsPanel.currentFocus = -1;
        }
    }

    Rectangle {
        id: root
        anchors.fill: parent
        Rectangle {
            id: settingsPanel
            width: 510
            height: contentItem.height
            anchors.horizontalCenter: parent.horizontalCenter

            opacity: 0.5

            property int currentFocus: -1;

            onCurrentFocusChanged: {
                controller.focusedElementIndex = currentFocus;
                console.log("Новый индекс " + currentFocus);
            }

            ScrollView {
                width: parent.width
                height: parent.height
                clip: true

                Column {
                    id: settingsColumn
                    width: parent.width
                    topPadding: 30

                    spacing: 10

                    Rectangle {
                        width: parent.width
                        height: buttonsSettingsTitle.height + 5
                        color: "bisque"

                        Text {
                            id: buttonsSettingsTitle
                            text: "Команды для кнопок"
                            leftPadding: 10
                            font.pointSize: 12;
                            font.bold: true
                        }
                    }

                    Repeater {
                        id: buttonCommandSettings

                        model: controller.buttonCommandsCount
                        delegate: RowLayout {
                            id: buttonSettingsRow
                            width: parent.width

                            Label {
                                id: label
                                text: controller.getButtonCommandName(index)
                                font.pointSize: 9
                                anchors.left: buttonSettingsRow.left
                            }
                            TextField {
                                id: textField
                                text: controller.getButtonName(index)
                                readOnly: true
                                anchors.right: buttonSettingsRow.right
                                font.pointSize: 9
                                horizontalAlignment: TextInput.AlignHCenter

                                MouseArea {
                                    id: buttonTextFieldArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: axesSettingsTitle.height + 5
                        color: "bisque"
                        Text {
                            id: axesSettingsTitle
                            text: "Команды для осей"
                            leftPadding: 10
                            font.pointSize: 12
                            font.bold: true
                        }
                    }

                    Repeater {
                        id: axisCommandSettings
                        model: controller.axisCommandsCount
                        delegate: RowLayout {
                            id: axisSettingsRow
                            width: parent.width
                            Label {
                                text: controller.getAxisCommandName(index)
                                anchors.left: axisSettingsRow.left
                                font.pointSize: 9
                            }
                            TextField {
                                id: textField2
                                text: controller.getAxisName(index)
                                anchors.right: axisSettingsRow.right
                                readOnly: true
                                focus: false
                                font.pointSize: 9
                                horizontalAlignment: TextInput.AlignHCenter
                                MouseArea {
                                    id: axisTextFieldArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: horizontalLine
                        width: parent.width
                        height: 1
                        color: "gray"
                    }

                    Button {
                         id:saveButton;
                         enabled: false;
                         text: "Сохранить";
                         anchors.right: settingsColumn.right;

                         Component.onCompleted: appMouseArea.buttonBellowClicked.connect(clicked)

                         onClicked: {
                             controller.saveSettings();
                         }
                     }
                }
            }
        }

        MouseArea {
            id: appMouseArea
            anchors.fill: parent
            enabled: false

            signal buttonBellowClicked

            onPressed: {
                if (isPointInsideButton(mouse.x, mouse.y)) {
                    saveButton.down = true
                }
            }

            onReleased: {
                if (isPointInsideButton(mouse.x, mouse.y)) {
                    buttonBellowClicked()
                }
                saveButton.down = false
            }

            function isPointInsideButton(x, y) {
                const mapped = root.mapToItem(saveButton, x, y)
                if (saveButton.contains(Qt.point(mapped.x, mapped.y))) {
                    return true
                }
                return false
            }

            onClicked: {
                var wasFocused = false;
                for (var i = 0; i < buttonCommandSettings.count; i++) {
                    var item = buttonCommandSettings.itemAt(i);
                    var textField = item.children[1];
                    var mouseArea = textField.children[1];
                    if (mouseArea.containsMouse) {
                        textField.forceActiveFocus();
                        wasFocused = true;
                        settingsPanel.currentFocus = i;
                        break;
                    }
                }
                for (var i = 0; i < axisCommandSettings.count; i++) {
                    var item = axisCommandSettings.itemAt(i);
                    var textField = item.children[1];
                    var mouseArea = textField.children[1];
                    if (mouseArea.containsMouse) {
                        textField.forceActiveFocus();
                        wasFocused = true;
                        settingsPanel.currentFocus = buttonCommandSettings.count + i;
                        break;
                    }
                }

                if (!wasFocused && settingsPanel.currentFocus != -1) {
                    var item;
                    if (settingsPanel.currentFocus < buttonCommandSettings.count) {
                        item = buttonCommandSettings.itemAt(settingsPanel.currentFocus);

                    } else {
                        item = axisCommandSettings.itemAt(settingsPanel.currentFocus % buttonCommandSettings.count);
                    }
                    var textField = item.children[1];
                    textField.focus = false;
                    settingsPanel.currentFocus = -1;
                }
            }
        }
    }
}
