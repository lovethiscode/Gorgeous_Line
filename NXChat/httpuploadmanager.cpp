#include "httpuploadmanager.h"
#include <QHttpMultiPart>
#include <QFile>
#include <QNetworkReply>
#include <QFileInfo>

HttpUploadManager::HttpUploadManager(QObject *parent) : QObject(parent)
{
}

HttpUploadManager *HttpUploadManager::Instance()
{
    static HttpUploadManager* s_instance = new HttpUploadManager;
    return s_instance;
}

void HttpUploadManager::SetProxy(const QNetworkProxy &proxy)
{
    manager_.setProxy(proxy);
}

void HttpUploadManager::UploadFile(const QString &url, const QString &path, const QString &params, const QMap<QString, QString> &heads, UploadDelegate delegate)
{
  QUrl qurl(url);
  QNetworkRequest request(qurl);

  QMapIterator<QString, QString> iter(heads);
  while (iter.hasNext()) {
      iter.next();
      QString key = iter.key();
      if(key == "Content-Type") {
          continue;
      }
      request.setRawHeader(key.toUtf8(), iter.value().toUtf8());
  }

  QFileInfo file_info(path);

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  if(!params.isEmpty()) {
    //添加参数
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"params\""));
    textPart.setBody(params.toUtf8());
     multiPart->append(textPart);
  }

  //添加文件
  QHttpPart file_part;
  QString file_part_header("form-data; name=\"files\"; filename=\"%1\"");
  file_part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(file_part_header.arg(file_info.fileName())));
  QFile *file = new QFile(path);
  file->open(QIODevice::ReadOnly);
  file_part.setBodyDevice(file);
  file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
  multiPart->append(file_part);


  QNetworkReply *reply = manager_.post(request, multiPart);
  multiPart->setParent(reply); // delete the multiPart with the reply
   connect(reply, &QNetworkReply::finished, this, [=](){
      //  接收数据
     if(delegate) {
       auto code = reply->error();
       if(code != QNetworkReply::NoError) {
           qDebug() << "upload error:" << reply->errorString();
       }
       delegate(code);
     }
      reply->deleteLater();
   });
}

void HttpUploadManager::DownloadFile(const QString &url, const QString &path, const QMap<QString, QString> &heads, UploadDelegate delegate)
{
    QUrl qurl(url);
    QNetworkRequest request(qurl);

    QMapIterator<QString, QString> iter(heads);
    while (iter.hasNext()) {
        iter.next();
        QString key = iter.key();
        if(key == "Content-Type") {
            continue;
        }
        request.setRawHeader(key.toUtf8(), iter.value().toUtf8());
    }

    QFile *file = new QFile(path);
    file->open(QIODevice::ReadWrite | QIODevice::Truncate);


    QNetworkReply *reply = manager_.post(request, "");
    file->setParent(reply);
    connect(reply, &QIODevice::readyRead, this, [=] (){
        QByteArray data = reply->readAll();
        file->write(std::move(data));
    });

     connect(reply, &QNetworkReply::finished, this, [=](){
        //  接收数据
       if(delegate) {
         auto code = reply->error();
         if(code != QNetworkReply::NoError) {
             qDebug() << "upload error:" << reply->errorString();
         }
         delegate(code);
       }
        reply->deleteLater();
     });
}
