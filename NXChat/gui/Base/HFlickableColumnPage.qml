// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.12

HPage {
    id: page

    default property alias columnData: column.data

    property int customMargin
    property alias column: column
    property alias flickable: flickable

    implicitWidth: theme.controls.box.defaultWidth
    implicitHeight: contentHeight + implicitHeaderHeight + implicitFooterHeight
    contentHeight:
        flickable.contentHeight + flickable.topMargin + flickable.bottomMargin

    padding: 0

    HFlickable {
        id: flickable
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: column.implicitHeight
        flickableDirection: Flickable.VerticalFlick
        clip: true

        topMargin: customMargin || theme.spacing
        bottomMargin: topMargin
        leftMargin: topMargin
        rightMargin: topMargin

        HColumnLayout {
            id: column
            width:
                flickable.width - flickable.leftMargin - flickable.rightMargin
            spacing: theme.spacing * 1.5
        }
    }

    HKineticScrollingDisabler {
        flickable: flickable
        width: enabled ? flickable.width : 0
        height: enabled ? flickable.height : 0
    }
}
