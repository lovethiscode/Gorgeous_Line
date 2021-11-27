// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import QtAV 1.7

OSD {
    id: osd

    property alias source: audioPlayer.source
    property alias autoLoad:  audioPlayer.autoLoad
    property alias volume:  audioPlayer.volume
    property alias startMuted:  audioPlayer.muted
    property alias autoPlay:  audioPlayer.autoPlay


    audioOnly: true
    media: audioPlayer

    implicitWidth: osd.width
    implicitHeight: osd.height

    MediaPlayer {
        id: audioPlayer
        autoLoad: true
        autoPlay: true
        volume: 0.5
        muted: false
    }
}
