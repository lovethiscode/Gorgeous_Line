// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import "../../Base"
import LineEngine 0.1
import EnumDefine 0.1

HTextArea {
    id: bubble
    property string session_id
    property var model

    anchors.margins: theme.spacing / 4
    readOnly: true
    topPadding: theme.spacing / 2
    bottomPadding: topPadding
    leftPadding: theme.spacing / 1.5
    rightPadding: leftPadding
    text: model.content
    enabled: model.status !== EnumDefine.EventItemStatusSending

    font.pixelSize: theme.fontSize.small

    EventStatus {
        anchors.fill: parent
        status: model.status
        onRetry: {
            LineEngine.reSendText(session_id, model.id, model.content);
        }
    }
}
