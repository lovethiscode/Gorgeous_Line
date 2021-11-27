import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtAV 1.7
import "../.."
import "../../Base"
import "../../Base/MediaPlayer"
import LineEngine 0.1
import EnumDefine 0.1

VideoPlayer {
    property string session_id
    property var model
    source: model.source
    enabled: model.status !== EnumDefine.EventItemStatusSending

    EventStatus {
        anchors.fill: parent
        status: model.status
        time: model.date
        onRetry: {
            LineEngine.reSendVideo(session_id, model.id, model.source)
        }
    }
}
