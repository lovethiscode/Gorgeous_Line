import QtQuick 2.12
import QtQuick.Layouts 1.15
import "../../Base"
import "../../Base/HTile"
import LineEngine 0.1

HTile {
    id: contact
    backgroundColor: theme.mainPane.listView.account.background
    contentItem: ContentRow {
        tile: contact

        HUserAvatar {
            id: avatar
            clientUserId: model.mid
            userId: model.mid
            displayName: model.display_name
            mxc: model.picture_path
            compact: contact.compact
        }

        HRowLayout {
            spacing: theme.spacing

            TitleLabel {
                text:  (model.display_name || model.mid)
                color:  utils.nameColor(model.display_name || model.mid.substring(0, 1))
                font.pixelSize: 16
                Behavior on color { HColorAnimation {} }
            }
        }

        //接受， 拒绝
        HButton {
            id: accept
            backgroundColor: "transparent"
            icon.name: "invite-accept"
            text: "Accept"
            onClicked: {            
                LineEngine.acceptGroupInvited(model.mid, true)
            }
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            visible: model.status === 0
        }

        HButton {
            id: reject
            backgroundColor: "transparent"
            icon.name: "invite-decline"
             text: "Reject"
            onClicked: {               
                LineEngine.acceptGroupInvited(model.mid, false)
            }
            visible: model.status === 0
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
        }


        TitleLabel {
            id: operatorTip
            visible: model.status === 1 || model.status === 2
            text:  model.status === 1 ? "Accepted" : "Rejected"
            color:  utils.nameColor(text)
            font.pixelSize: 16
            horizontalAlignment: TextEdit.AlignRight
        }

        HButton {
             text: "Delete"
            onClicked: {
                window.mainUI.showToast(qsTr("Double Click to Delete"), 2000)
            }
            onDoubleClicked: {
                LineEngine.deleteGroupInvited(model.mid)
            }

            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
        }
    }
}
