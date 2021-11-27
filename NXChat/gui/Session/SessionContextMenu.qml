import QtQuick 2.0
import "../Base"


HMenu {
    id: menu
    property QtObject model: null
    property int row: 0
    signal removeSession(QtObject session, int row)

    function spawn(model, row) {
        menu.model = model
        menu.row = row
        menu.popup()
    }


    HMenuItem {
        icon.name: "presence-online"
        icon.color: theme.controls.presence.online
        text: qsTr("Remove Session")
        onTriggered: {
            removeSession(menu.model, row)
        }
    }
}
