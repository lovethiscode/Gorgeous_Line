// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../.."
import "../../Base"
import LineEngine 0.1
import EnumDefine 0.1

HMxcImage {
    id: image
    property string session_id
    property var model
    property alias media_url : image.source
    fillMode: Image.PreserveAspectFit
    sourceSize.width: 128
    sourceSize.height: 128
    source : model.source || "qrc:/resource/images/default-preview.jpg"
    enabled: model.status !== EnumDefine.EventItemStatusSending

    Layout.preferredHeight:
        status === Image.Ready ?
        width / (implicitWidth / implicitHeight) :
        96 * theme.uiScale  // for spinner

    Behavior on Layout.preferredHeight { HNumberAnimation {} }

    EventStatus {
        anchors.fill: parent
        status: model.status
        time: model.date
        onRetry: {
            LineEngine.reSendImage(session_id, model.id, model.source)
        }
    }
}
