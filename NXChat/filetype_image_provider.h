// Copyright Mirage authors & contributors <https://github.com/mirukana/mirage>
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QImage>
#include <QFileInfo>
#include <QQuickImageProvider>
#include <QMimeType>
#include <QFileIconProvider>
#include <QMimeDatabase>
#include "util/fileutil.h"

class FileTypeImageProvider : public QQuickImageProvider {
public:
    explicit FileTypeImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {

    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
        int width = requestedSize.width() > 0 ? requestedSize.width() : 64;
        int height = width;
        if (size) *size = QSize(width, height);
        return icon_provider_.icon(QFileInfo(FileUtil::ToStandardPath(id))).pixmap(width, height);
    }

private:
    QFileIconProvider icon_provider_;
};

