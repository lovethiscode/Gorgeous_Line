// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Window 2.12
import QtAV 1.7

Video {
    id: video
    autoLoad: false
    autoPlay: false
    volume: 0.5
    muted: false
    implicitWidth: 300
    implicitHeight: width / osd.savedAspectRatio

    property bool hovered: false

    property int oldVisibility: Window.Windowed
    property QtObject oldParent: video.parent

    MouseArea {
        width: parent.width
        height: parent.height - (osd.visible ? osd.height : 0)
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        propagateComposedEvents: true

        onContainsMouseChanged: video.hovered = containsMouse
        onMouseXChanged: osd.showup = true
        onMouseYChanged: osd.showup = true
    }

    OSD {
        id: osd
        width: parent.width
        anchors.bottom: parent.bottom
    }
}
