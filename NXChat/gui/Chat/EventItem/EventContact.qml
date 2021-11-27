import QtQuick 2.0
import QtQuick.Layouts 1.15
import "../../Base"
import "../../Base/HTile"

HTile {
    id: contact
    property string session_id
    property var model

    contentItem: ContentRow {
        tile: contact


        HButton {
            circle: true
            icon.name: "copy-user-id"
            backgroundColor: "transparent"
        }

        TitleLabel {
            elide: Text.ElideNone
            text: model.title
        }


    }
}
