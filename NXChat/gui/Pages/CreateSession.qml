import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../.."
import "../Base"
import "../Base/Buttons"
import "Contacts"
import "Groups"

HPage {
    HTabbedBox {
        anchors.centerIn: parent
        width: 450
        

        tabBar: HTabBar {
            HTabButton { text: qsTr("Contacts") }
            HTabButton { text: qsTr("Groups") }
        }

        Contacts {  }
        Groups {}
    }
}
