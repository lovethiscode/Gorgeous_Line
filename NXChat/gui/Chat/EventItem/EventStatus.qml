import QtQuick 2.0
import "../../Base"
import "../../Base/MediaPlayer"
import QtQuick.Layouts 1.15
import EnumDefine 0.1

Item {
    property int status
    property alias time: timestamp.text
    onStatusChanged: {
        loading.source = status === EnumDefine.EventItemStatusSending ? "qrc:/gui/LoadingScreen.qml" : ""
    }

    signal retry()
    HLoader {
        id : loading
        anchors.fill: parent
    }
    HButton {
        circle: true
        backgroundColor: "red"
        icon.name: "retry"
        visible: status === EnumDefine.EventItemStatusFailed
         x:  -2 - width
         anchors.bottom: parent.bottom
         onClicked: {
            retry()
        }
    }
    HLabel {
        id : timestamp
        anchors.top: parent.top
        anchors.right: parent.right

    }
}
