// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../.."
import "../../Base"
import "../../Base/Buttons"
import "../../Dialogs"
import LineEngine 0.1

HFlickableColumnPage {
    id: page

    property var profile
    readonly property bool ready: true

    function takeFocus() {
        nameField.item.forceActiveFocus()
    }


    function onCommandResponse(command, json) {
        if(command === "SetSignature") {
            saveButton.signatureChangeRunning = false
        } else if(command === "SetNickName") {
            saveButton.nameChangeRunning = false
        }
    }
    Component.onDestruction: {
        LineEngine.commandResponse.disconnect(onCommandResponse)
    }

    Component.onCompleted: {
        //  连接回包信号
        LineEngine.commandResponse.connect(onCommandResponse)
    }

    function applyChanges() {
        if (avatar.changed) {
            saveButton.avatarChangeRunning = true

            const path =
                Qt.resolvedUrl(avatar.sourceOverride).replace(/^file:/, "")
            console.log(path)
            //更新头像
            //saveButton.avatarChangeRunning = false
        }

        if (nameField.item.changed) {
            saveButton.nameChangeRunning = true
            const name                   = nameField.item.text
            LineEngine.setNickname(name)
            //更新显示名称
        }

        if(signatureField.item.changed) {
            saveButton.signatureChangeRunning = true;
            LineEngine.setSignature(signatureField.item.text)
        }
    }

    function cancel() {
        window.mainUI.showPage("", {})
    }

    footer: AutoDirectionLayout {
        ApplyButton {
            id: saveButton

            property bool nameChangeRunning: false
            property bool avatarChangeRunning: false
            property bool signatureChangeRunning: false

            disableWhileLoading: false
            loading:
                nameChangeRunning ||
                avatarChangeRunning ||
                signatureChangeRunning

            enabled:
                avatar.changed ||
                nameField.item.changed ||
                signatureField.item.changed

            onClicked: applyChanges()
        }

        CancelButton {
            enabled: ! saveButton.loading
            onClicked: cancel()
        }
    }

    onKeyboardAccept: if (saveButton.enabled) saveButton.clicked()
    onKeyboardCancel: cancel()

    HUserAvatar {
        id: avatar

        property bool changed: Boolean(sourceOverride)

        clientUserId: profile.userid
        userId: profile.userid
        displayName: nameField.item.text
        mxc: profile.thumbnailUrl ? profile.thumbnailUrl : ""
        toolTipMxc: ""
        sourceOverride: fileDialog.selectedFile || fileDialog.file

        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
        // Layout.preferredWidth: 256 * theme.uiScale
        Layout.preferredHeight: width

        Rectangle {
            id: changeAvatarTip
            anchors.fill: parent
            z: 10
            visible: avatar.hovered
            color: utils.hsluv(
                0, 0, 0, (! avatar.mxc && overlayHover.hovered) ? 0.8 : 0.7,
            )

            Behavior on color { HColorAnimation {} }

            HoverHandler { id: overlayHover }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape:
                    overlayHover.hovered ?
                    Qt.PointingHandCursor : Qt.ArrowCursor
            }

            HLoader {
                anchors.centerIn: parent
                width: avatar.width / 3
                height: width

                source: "../../Base/HBusyIndicator.qml"
                active: ! ready
                opacity: active ? 1 : 0
                visible: opacity > 0

                Behavior on opacity { HNumberAnimation {} }
            }

            HColumnLayout {
                anchors.centerIn: parent
                spacing: currentSpacing
                width: parent.width
                opacity: ready ? 1 : 0
                visible: opacity > 0

                Behavior on opacity { HNumberAnimation {} }

                HIcon {
                    svgName: "upload-avatar"
                    colorize: (! avatar.mxc && overlayHover.hovered) ?
                              theme.colors.accentText : theme.icons.colorize
                    dimension: avatar.width / 3

                    Layout.alignment: Qt.AlignCenter
                }

                Item { Layout.preferredHeight: theme.spacing }

                HLabel {
                    text: avatar.mxc ?
                          qsTr("Change profile picture") :
                          qsTr("Upload profile picture")

                    color: (! avatar.mxc && overlayHover.hovered) ?
                           theme.colors.accentText : theme.colors.brightText
                    Behavior on color { HColorAnimation {} }

                    font.pixelSize: Math.max(
                        theme.fontSize.big * avatar.width / 300,
                        theme.fontSize.small,
                    )
                    wrapMode: HLabel.WordWrap
                    horizontalAlignment: Qt.AlignHCenter

                    Layout.fillWidth: true
                }
            }
        }

        HFileDialogOpener {
            id: fileDialog
            dialog.modality: Qt.WindowModal
            enabled: ready
            fileType: HFileDialogOpener.FileType.Images
            dialog.title: qsTr("Select profile picture for %1")
                              .arg(profile ? profile.displayName : "")
            onFilePicked: {
                avatar.mxc = file
            }
        }
    }

    HLabeledItem {
        label.text: qsTr("User ID:")

        Layout.fillWidth: true

        HRowLayout {
            width: parent.width

            HTextArea {
                id: idArea
                wrapMode: HLabel.Wrap
                readOnly: profile.userid ? true : false
                radius: 0
                text: profile.userid

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            FieldCopyButton {
                textControl: idArea              
                visible: profile.userid ? true : false
            }

            HButton {
                icon.name: "check-mark"
                iconItem.small: true
                Layout.fillHeight: true
                visible : profile.userid ? false : true
                enabled: idArea.displayText !== "" ? true : false

                onClicked: {
                    console.log(idArea.displayText)
                }
             }
        }
    }

    HLabeledItem {
        id: nameField
        loading: ! ready
        label.text: qsTr("Display name:")

        Layout.fillWidth: true

        HTextField {
            width: parent.width
            defaultText: ready ? profile.displayName : ""
            maximumLength: 255

            // TODO: Qt 5.14+: use a Binding enabled when text not empty
            color: utils.nameColor(text)
        }
    }

    HLabeledItem {
        id: signatureField
        loading: ! ready
        label.text: qsTr("Status Message:")

        Layout.fillWidth: true

        HTextField {
            width: parent.width
            defaultText: ready ? profile.statusMessage : ""
            maximumLength: 255

            // TODO: Qt 5.14+: use a Binding enabled when text not empty
            color: utils.nameColor(text)
        }
    }

    HLabeledItem {
        label.text: qsTr("Ticket:")
        Layout.fillWidth: true

        HRowLayout {
            width: parent.width

            HTextField {
                wrapMode: HLabel.Wrap
                readOnly: true
                radius: 0
                defaultText: profile.qrcode

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            HButton {
                icon.name: "retry"
                iconItem.small: true
                Layout.fillHeight: true

                toolTip.text: qsTr("Frush")
                onClicked: {
                    LineEngine.getQrcode();
                }
             }
        }
    }
}
