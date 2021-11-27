// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import LineEngine 0.1
import "../.."
import "../../Base"

HPage {
    id: page

    property var profile

    function onProfileChanged() {
        profile = LineEngine.profile
    }
    Component.onDestruction: {
        LineEngine.notifyProfileChanged.disconnect(onProfileChanged)
    }

    Component.onCompleted: {
        //  连接回包信号
        LineEngine.notifyProfileChanged.connect(onProfileChanged)
    }

    HTabbedBox {
        anchors.centerIn: parent
        width: Math.min(implicitWidth, page.availableWidth)
        height: Math.min(implicitHeight, page.availableHeight)

        tabBar: HTabBar {
            HTabButton { text: qsTr("General") }
           // HTabButton { text: qsTr("Notifications") }
           // HTabButton { text: qsTr("Security") }
        }

        General { profile: page.profile }
        /*Item {

        }
        Item {

        }*/
    }
}
