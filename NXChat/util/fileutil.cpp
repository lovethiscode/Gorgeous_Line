#include "fileutil.h"
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QVariant>


QString FileUtil::ToStandardPath(const QString &uri_path)
{
    QString result = uri_path;
    result.replace("file:///", "");
    return result;
}

bool FileUtil::FileExist(const QString &path)
{
    QString normal_path = ToStandardPath(path);
    QFileInfo info(normal_path);
    return info.exists();
}

QString FileUtil::SaveImageToFile(const QVariant &image)
{
  QDir temp_dir(QDir::tempPath());
  QString file_name = QString::number(QDateTime::currentMSecsSinceEpoch()) + ".png";
  QString full_path = temp_dir.absoluteFilePath(file_name);
  QFile file(full_path);
  file.open(QIODevice::ReadWrite | QIODevice::Truncate);
  file.write(image.toByteArray());

  return full_path;
}

int FileUtil::GetMediaLength(const QString &path)
{
  return 1000;
}
