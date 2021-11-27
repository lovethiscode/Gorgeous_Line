import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Base"
import EventModel 0.1
import LineEngine 0.1

Rectangle {
    id: root
    property QtObject session;
    anchors.fill: parent
    color: theme.chat.eventList.background

    readonly property alias contextMenu: contextMenu

    HListView {
        function copySelectedDelegates() {

        }

        id: eventList
        anchors.fill: parent
        clip: true
        leftMargin: theme.spacing
        rightMargin: theme.spacing
        topMargin: theme.spacing
        bottomMargin: theme.spacing
        spacing: 5
        verticalLayoutDirection: ListView.BottomToTop
        readonly property alias contextMenu: contextMenu

        model: LineEngine.getEventModel(root.session.id)
        delegate: EventDelegate {
            width:  parent ? parent.width ? parent.width : parent.implicitWidth : 0
            session: root.session
        }


        EventDelegateMenu {
            id: contextMenu
        }
    }


    HNoticePage {
        text: qsTr("No messages to show yet")

        visible: eventList.model.count < 1
        anchors.fill: parent
    }
}
