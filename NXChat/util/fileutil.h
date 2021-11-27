#ifndef FILE_UTIL_H
#define FILE_UTIL_H
#include <QString>
#include <QObject>

class FileUtil :public QObject {
    Q_OBJECT

public:
    static QString ToStandardPath(const QString& uri_path);
    Q_INVOKABLE bool FileExist(const QString& path);

    static QString SaveImageToFile(const QVariant& image);

    static int GetMediaLength(const QString& path);
};

#endif // FILE_UTIL_H
