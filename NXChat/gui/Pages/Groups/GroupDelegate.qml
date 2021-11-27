import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../../Base"
import "../../Base/HTile"

HTile {
    id: group
    backgroundColor: "white"

    contentItem: ContentRow {
        tile: group

        HUserAvatar {
            id: avatar
            clientUserId: model.id
            userId: model.id
            displayName: model.display_name
            mxc: model.picture_path
            compact: group.compact
        }

        HColumnLayout {
            HRowLayout {
                spacing: theme.spacing

                TitleLabel {
                    text:  model.display_name || model.id

                    color:  utils.nameColor(model.display_name || model.id.substring(0, 1))

                    Behavior on color { HColorAnimation {} }
                }
            }

            /*SubtitleLabel {
                tile: group
                textFormat: SubtitleLabel.PlainText
                color: theme.chat.roomPane.listView.member.subtitle
                text: (model.status_message.trim() || "")
            }*/
        }
    }
}
