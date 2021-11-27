// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.12
import CppUtils 0.1
import "../.."
import "../../Base"
import "../../Base/HTile"
import LineEngine 0.1
import FileUtil 0.1
import EnumDefine 0.1

HTile {
    id: file
    property string source: model.source
    property string session_id
    property var model

    height: Math.max(theme.chat.message.avatarSize, implicitHeight)

    contentItem: ContentRow {
        tile: file
        Image {
            sourceSize.width: 32
            sourceSize.height: 32
            source: "image://icon/" + file.source
        }


        HColumnLayout {
            TitleLabel {
                elide: Text.ElideMiddle
                text: model.title
            }

            SubtitleLabel {
                tile: file
                text: CppUtils.formattedBytes(model.size,
                )
            }
        }
        HIcon {
            svgName: "download"
        }
    }

    onMiddleClicked: leftClicked()
    onLeftClicked:  {
        if (FileUtil.FileExist(source)){
            Qt.openUrlExternally(source)
        } else {
            LineEngine.downloadMedia(session_id, model.id, model.title)
        }

    }

    enabled: model.status !== EnumDefine.EventItemStatusSending


    EventStatus {
        anchors.fill: parent
        status: model.status
        time: model.date
        onRetry: {
             LineEngine.reSendFile(session_id, model.id, model.source)
        }
    }
}
