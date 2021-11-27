#pragma once
#include <QString>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include "enum_define.h"

#define SETTING_QRCODE "qrcode"

enum AccountType {
    AccountTypeEmail,
    AccountTypePhone
};

struct AccountInfo {
    QString account;
    QString password;
    QString token;
    AccountType type;
};

enum MidType {
  MidTypeUnknow,
  MidTypeUser,
  MidTypeRoom,
  MidTypeGroup,
  MidTypeSquare,
  MidTypeSquareChat,
  MidTypeSquareMember,
  MidTypeBot
};

MidType GetMidType(const QString& mid);


template<class T>
QVariantList ToList(const QList<T>& data) {
  QVariantList result;
  for(auto& item : data) {
    result.append(item);
  }
  return result;
}

struct ContactItem {
    static int s_uniqueid;
    int uniqueid = 0;
    QString mid;
    QString display_name;
    QString thumbnail_url;
    QString picture_path;   
    QString status; //friend
    QString status_message;
    QString type;
    QString relation;
    ContactItem() {
        uniqueid = s_uniqueid++;
    }
    QVariantMap ToMap();

    static ContactItem FromMap(const QVariantMap& map);

    static QList<ContactItem> FromJsonArray(const QJsonArray& contacts);

    bool operator< (const ContactItem& item) {
        //转成大写比较
        QString source_upper = display_name.toUpper();
        QString other_upper = item.display_name.toUpper();

        if(source_upper != other_upper) {
            return source_upper < other_upper;
        }
        return uniqueid < item.uniqueid;
    }
};

struct GroupItem {
  static int s_uniqueid;
  int uniqueid = 0;
    QString mid;
    QString display_name;
    QString picture_path;
    QString creator;
    QString ticket;
    QList<QString> members;
    QList<QString> invitess;
    bool join_by_ticket = false;
    GroupItem() {
        uniqueid = s_uniqueid++;
    }
    static QList<GroupItem> FromJsonArray(const QJsonArray& groups);
    QVariantMap ToMap();
    bool operator< (const GroupItem& item) {
        //转成大写比较
        QString source_upper = display_name.toUpper();
        QString other_upper = item.display_name.toUpper();

        if(source_upper != other_upper) {
            return source_upper < other_upper;
        }
        return uniqueid < item.uniqueid;
    }
};

struct InvitedIntoGroupItem {
    GroupItem group_item;
    int status = 0; // 0 未处理  1 接受  2 拒绝
    QString invitee;
    int64_t timestamp = 0;
    QVariantMap ToMap();
    bool operator== (const InvitedIntoGroupItem& other) {
        return group_item.mid == other.group_item.mid;
    }
};

struct SessionItem {
    QString id;
    QString display_name;
    QString presence; //offline  invisible
    int total_unread = 0;
    QString avatar_url;
    QString status_msg;
    EnumDefine::SessionType type = EnumDefine::SessionTypePrivate;
    int timestamp = 0;

    static SessionItem FromContactMap(const QVariantMap& map);
    bool operator== (const SessionItem& other) {
        return id == other.id;
    }
};

struct EventItemData {
    QString id;
    QString sender_id;
    QString sender_name;
    QString content;
    QString avatar_url;
    bool isMe = false;
    int timestamp = 0;
    EnumDefine::EventType type = EnumDefine::EventTypeText;
    QString source;
    QString title;
    quint64 size = 0;
    EnumDefine::EventItemStatus status = EnumDefine::EventItemStatusInit;
    bool operator== (const EventItemData& other) {
        return id == other.id;
    }
    QByteArray ToByteArray() const;
    static EventItemData FromByteArray(const QByteArray& data);
    static EventItemData FromJsonObject(const QJsonObject& message);
};

enum AbstractEventRoleName {
    Id,
    SenderId,
    SenderName,
    Content,
    AvatarUrl,
    IsMe,
    Date,
    Type,
    Source,
    Title,
    Size,
    Status
};
