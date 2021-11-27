// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12
import "Base"

Rectangle {
    color: utils.hsluv(0, 0, 0, 0.7)

    HBusyIndicator {
        anchors.centerIn: parent
        width: Math.min(32, parent.width, parent.height - 4)
        height: width

        // Because the theme is not loaded at this point, we must set these
        // properties manually:
        baseCircle.strokeColor: utils.hsluv(240, 60 / 1.5 * 2, 0, 0.7)
        progressCircle.strokeColor: utils.hsluv(240, 60 * 1.5, 72)
        label.font.family: "Roboto"
        label.font.pixelSize: 0
        label.color: "black"
        label.linkColor: "black"

    }
}
