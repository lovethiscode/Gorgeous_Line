import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import LineEngine 0.1
import "../../Base"
import "../../Base/Buttons"
import "../../Base/HTile"
import ".."

PageBase {
    id: page
    title : "Group Invite"
    applyBtnVisible : false
    cancelBtnText: qsTr("Close")
    cancelBtnIcon : ""
    fixWidth : 400
    customMargin: 1
    Component.onDestruction: {
        LineEngine.deleteInvitedGroupsModel();
    }
    onCancelBtnClicked :{
        window.mainUI.showPage("", {})
    }
     content : Page {
         implicitHeight: 500
         background: theme.controls.button.background
         HListView {
             id : groupList
             anchors.fill: parent
             clip: true
             model : LineEngine.getInvitedGroupsModel()
             delegate: GroupInviteDelegate {
                  width:  groupList.width
                  height: theme.controls.avatar.size + theme.spacing
                  onLeftClicked: {
                      groupList.currentIndex = model.index
                  }
             }


             highlight: Item {
                 readonly property alias border: border

                 Rectangle {
                     anchors.fill: parent
                     color: theme.mainPane.accountBar.account.selectedBackground
                     opacity: theme.mainPane.accountBar.account
                                   .selectedBackgroundOpacity
                 }

                 Rectangle {
                     id: border
                     anchors.bottom: parent.bottom
                     width: parent.width
                     height:
                         theme.mainPane.accountBar.account.selectedBorderSize
                     color: theme.mainPane.accountBar.account.selectedBorder
                 }
             }
         }
     }
}
