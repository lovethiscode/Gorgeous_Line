import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import LineEngine 0.1
import "../../Base"
import "../../Base/Buttons"
import "../../Base/HTile"
import ".."
HFlickableColumnPage {
    function takeFocus() {
    }

    implicitHeight: 600
    id: page
    function closePage() {
         window.mainUI.showPage("", {})
    }

    function applyChanges() {
        window.mainUI.showPage("qrc:/gui/Pages/SearchContact.qml", {})

    }

         header: Rectangle {
             height: 50
             color: "red"
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
             id : contactList
             anchors.fill: parent
             implicitHeight: 520
             clip: true
             model : LineEngine.getContactModel()
             section.property: "display_name"
             section.criteria: ViewSection.FirstCharacter
             section.delegate: sectionDelegate
             delegate: ContactDelegate {
                  width:  contactList.width
                  height: theme.controls.avatar.size + theme.spacing

                   onLeftClicked:{                     
                       LineEngine.addSession(model.id, 0)
                   }
             }
         }

     footer: AutoDirectionLayout {
         ApplyButton {
             id: saveButton
             text: qsTr("Add Contact")
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
