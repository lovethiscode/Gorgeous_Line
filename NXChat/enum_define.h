#ifndef ENUM_DEFINE_H
#define ENUM_DEFINE_H

#include <QObject>

class EnumDefine : public QObject
{
    Q_OBJECT
public:
    //给qml 使用
    enum SessionType {
        SessionTypePrivate,
        SessionTypeGroup
    };
    Q_ENUM(SessionType)


    //qml 要使用枚举，这里需要有一个对应的
    enum EventType {
        EventTypeText,
        EventTypeImage,
        EventTypeAudio,
        EventTypeVideo,
        EventTypeFile,
        EventTypeContact,
        EventTypeVcard
    };
      Q_ENUM(EventType)


    enum EventItemStatus {
      EventItemStatusInit,
      EventItemStatusSuccess,
      EventItemStatusSending,
      EventItemStatusFailed,
      EventItemStatusDownloading
    };
    Q_ENUM(EventItemStatus)

    explicit EnumDefine(QObject *parent = nullptr);

signals:

};

#endif // ENUM_DEFINE_H
