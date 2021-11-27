import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Base"
import "../Base/HTile"
import "../Chat/Composer"

Page {
    id: root
    property QtObject session;


    header: ConversationHeader {
        id: header
        session: root.session
        Layout.fillWidth: true
    }


    EventList {
        id: eventList
        session : root.session
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    footer: Composer {
        id: composer

        Layout.fillWidth: true
        Layout.maximumHeight: parent.height / 2
        height: 120
    }
}
