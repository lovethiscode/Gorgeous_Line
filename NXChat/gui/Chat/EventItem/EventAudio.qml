import QtAV 1.7
import QtQuick 2.15
import "../../Base"
import "../../Base/MediaPlayer"
import LineEngine 0.1
import EnumDefine 0.1

AudioPlayer {
    property string session_id
    property var model
    source: model.source
    autoPlay : false
    autoLoad : false
    readonly property bool hovered: hover.hovered
    enabled: model.status !== EnumDefine.EventItemStatusSending

    HoverHandler { id: hover }

    EventStatus {
        anchors.fill: parent
        status: model.status
        time: model.date
        onRetry: {
             LineEngine.reSendAudio(session_id, model.id, model.source);
        }
    }
}
