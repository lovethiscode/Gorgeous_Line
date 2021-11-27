// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../.."
import "../../Base"
import ".."
import "../../Base/Buttons"
import "../../Dialogs"
import LineEngine 0.1

Rectangle {
    id: root
    readonly property bool parted: true
    readonly property bool hasFocus:
        messageArea.activeFocus

    readonly property alias messageArea: messageArea

    function takeFocus() {
        messageArea.forceActiveFocus()
    }

    implicitHeight: Math.max(theme.baseElementsHeight, row.implicitHeight)
    color: "gray"

    HColumnLayout {
        id: row
        anchors.fill: parent

        //操作按钮
        HRowLayout {
            Layout.preferredHeight: 24
            Layout.maximumHeight: 28
            //顶部右边的操作栏
            ChatHeaderButton {
                text: qsTr("SendImage")
                leftPadding: topPadding
                rightPadding: topPadding


                backgroundColor: "transparent"
                icon.name: "room-header-copy"

                onClicked: {
                    fileDialog.fileType =  HFileDialogOpener.FileType.Images
                    fileDialog.dialog.title = qsTr("Select Image")
                    fileDialog.visible = true
                    fileDialog.dialog.open()
                }
            }

            ChatHeaderButton {
                text: qsTr("SendVideo")
                leftPadding: topPadding
                rightPadding: topPadding


                backgroundColor: "transparent"
                icon.name: "room-header-copy"

                onClicked: {
                    fileDialog.fileType = HFileDialogOpener.FileType.Videos
                    fileDialog.dialog.title = qsTr("Select Video")
                    fileDialog.visible = true
                    fileDialog.dialog.open()
                }
            }

            ChatHeaderButton {
                text: qsTr("SendAudio")
                leftPadding: topPadding
                rightPadding: topPadding


                backgroundColor: "transparent"
                icon.name: "room-header-copy"

                onClicked: {
                    fileDialog.fileType = HFileDialogOpener.FileType.Audios
                    fileDialog.dialog.title = qsTr("Select Audio")
                    fileDialog.visible = true
                    fileDialog.dialog.open()
                }
            }
            ChatHeaderButton {
                text: qsTr("SendFile")
                leftPadding: topPadding
                rightPadding: topPadding


                backgroundColor: "transparent"
                icon.name: "room-header-copy"

                onClicked: {
                    fileDialog.fileType = HFileDialogOpener.FileType.All
                    fileDialog.dialog.title = qsTr("Select File")
                    fileDialog.visible = true
                    fileDialog.dialog.open()
                }
            }

            HFileDialogOpener {
                id: fileDialog
                dialog.modality: Qt.WindowModal
                visible: false
                onFilePicked: {
                    if (fileType ===  HFileDialogOpener.FileType.Images) {
                      LineEngine.sendImage(eventList.session.id ,Qt.resolvedUrl(file).replace("file:///", ""));
                    } else if(fileType === HFileDialogOpener.FileType.Videos){
                      LineEngine.sendVideo(eventList.session.id ,Qt.resolvedUrl(file).replace("file:///", ""));
                    } else if(fileType === HFileDialogOpener.FileType.Audios){
                       LineEngine.sendAudio(eventList.session.id ,Qt.resolvedUrl(file).replace("file:///", ""));
                    } else if(fileType === HFileDialogOpener.FileType.All){
                        LineEngine.sendFile(eventList.session.id ,Qt.resolvedUrl(file).replace("file:///", ""));
                     }
                     visible = false
                }

                onCancelled: {
                    visible = false
                }
            }
        }

        //发送的消息框
        HScrollView {
            enabled: visible
            visible: true
            onVisibleChanged: if (root.hasFocus) root.takeFocus()

            Layout.fillHeight: true
            Layout.fillWidth: true

            MessageArea {
                id: messageArea
            }
        }
    }
}
