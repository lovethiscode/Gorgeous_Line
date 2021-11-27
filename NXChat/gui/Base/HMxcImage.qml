// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.12

HImage {
    id: image

    property string clientUserId
    property string mxc
    property string title
    property var fileSize: undefined  // undefined or int (bytes)

    property string sourceOverride: ""

    property string cachedPath: ""
    property bool canUpdate: true
    property bool show: ! canUpdate

    readonly property bool isMxc: mxc.startsWith("mxc://")

    function reload() {
        if (! canUpdate) return  // component was destroyed

        const w = sourceSize.width || width
        const h = sourceSize.height || height

        if (! image.mxc || w < 1 || h < 1 ) {
            show = false
            return
        }

        if (! isMxc) {
            if (source !== mxc) source = mxc
            show = image.visible
            return
        }
    }

    source: sourceOverride || (show ? cachedPath : "")
    showProgressBar:
        (isMxc && status === Image.Null) || status === Image.Loading

    onWidthChanged: Qt.callLater(reload)
    onHeightChanged: Qt.callLater(reload)
    onVisibleChanged: Qt.callLater(reload)
    onMxcChanged: Qt.callLater(reload)
}
