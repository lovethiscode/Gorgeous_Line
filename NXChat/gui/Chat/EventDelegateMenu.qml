import QtQuick 2.0
import ".."
import "../Base"

import EnumDefine 0.1
import Clipboard 0.1

HMenu {
    id: menu
    property QtObject model: null

    function spawn(model) {
        menu.model = model
        menu.popup()
    }

    onModelChanged: {
        if(!model) {
            return
        }

        copyLocalPath.visible = model.type !== EnumDefine.EventTypeText
        copyText.visible = model.type === EnumDefine.EventTypeText
    }

    HMenuItem {
        id: copyLocalPath
        visible:  false
        icon.name: "copy-local-path"
        text: qsTr("Copy local path")
        onTriggered:
            Clipboard.text = model.source
    }

    HMenuItem {
        id: copyText
        visible: false
        icon.name: "copy-text"
        text: qsTr("Copy text")

        onTriggered: {
            Clipboard.text = model.content
        }
    }

    onClosed: {
       model = null;
    }
}
