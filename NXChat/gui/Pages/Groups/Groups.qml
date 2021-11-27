import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import LineEngine 0.1
import "../../Base"
import "../../Base/Buttons"
import "../../Base/HTile"
import ".."
HFlickableColumnPage {
    implicitHeight: 600
    id: page

    function takeFocus() {
    }

    function closePage() {
         window.mainUI.showPage("", {})
    }

    function applyChanges() {
        window.mainUI.showToast(qsTr("Wait for implement"), 5000)
    }

     Component {
          id: sectionDelegate
          TitleLabel {
              font.pixelSize: theme.fontSize.bigger
              width: ListView.view.width
              height: 25
              text: section.substring(0, 1)
              color: theme.mainPane.listView.account.name
              horizontalAlignment: Qt.AlignHCenter
          }
      }

     HListView {
         id : groupList
         anchors.fill: parent
         implicitHeight: 520
         clip: true
         model : LineEngine.getGroupModel()
         section.property: "display_name"
         section.delegate: sectionDelegate
         section.criteria: ViewSection.FirstCharacter
         delegate: GroupDelegate {
              width:  groupList.width
              height: theme.controls.avatar.size + theme.spacing

               onLeftClicked:{
                   LineEngine.addSession(model.id, 1)
               }
         }
     }

     footer: AutoDirectionLayout {
         ApplyButton {
             id: saveButton
             text: qsTr("JoinGroup")
             icon.name: "add-chat"
             onClicked: applyChanges()
         }

         CancelButton {
             text: qsTr("Close")
             onClicked: {
                 closePage();
             }
         }
     }
}
