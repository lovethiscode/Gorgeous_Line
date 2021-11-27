#ifndef HTTPUPLOADMANAGER_H
#define HTTPUPLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

typedef std::function<void(int code)> UploadDelegate;

class HttpUploadManager : public QObject
{
    Q_OBJECT
public:
    static HttpUploadManager* Instance();
    void SetProxy(const QNetworkProxy& proxy);
    void UploadFile(const QString& url, const QString& path, const QString& params, const QMap<QString, QString>& heads, UploadDelegate delegate);

    void DownloadFile(const QString& url, const QString& path, const QMap<QString, QString>& heads, UploadDelegate delegate);

signals:


private:
    explicit HttpUploadManager(QObject *parent = nullptr);
    QNetworkAccessManager manager_;
};

#endif // HTTPUPLOADMANAGER_H
