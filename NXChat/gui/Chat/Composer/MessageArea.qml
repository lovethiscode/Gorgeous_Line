// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
 import QtQuick.Layouts 1.15
import Clipboard 0.1
import "../.."
import "../../Base"
import LineEngine 0.1

HTextArea {
    id: area
    color: "black"
    placeholderTextColor: "black"
    Layout.fillWidth: true
    placeholderText: qsTr("Type a message...")
    tabStopDistance: 4 * 4  // 4 spaces
    backgroundColor: "white"
    //focusedBorderColor: "transparent"
    enableCustomImagePaste: true
    onCustomImagePaste: {
        window.makePopup(
                "Popups/ConfirmClipboardUploadPopup.qml",
                {
                    display_name: eventList.session.display_name,
                    doneCallback: (image) => {
                            LineEngine.sendImageData(eventList.session.id, image)
                     }
                }
            )
    }

    Keys.onReturnPressed: ev => {
        sendText()
    }

    function sendText() {
        LineEngine.sendText(eventList.session.id, text)
        area.clear()
    }

    //发送按钮
    HButton {
        icon.name: "confirm-uploading-file"
        backgroundColor: "transparent"
        onClicked: {
            sendText()
        }
       anchors.right: parent.right
       anchors.bottom: parent.bottom
       enabled: area.text
    }
}
