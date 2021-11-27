#include "modelitem.h"

#include <QBuffer>
#include <QDataStream>
#include "lineengine.h"

int ContactItem::s_uniqueid = 0;
int GroupItem::s_uniqueid = 0;

QVariantMap ContactItem::ToMap()
{
   QVariantMap map;
   map["mid"] = mid;
   map["display_name"] = display_name;
   map["thumbnail_url"] = thumbnail_url;
   map["picture_path"] = picture_path;
   map["type"] = type;
   map["status"] = status;
   map["relation"] = relation;
   map["status_message"] = status_message;
   return map;
}

ContactItem ContactItem::FromMap(const QVariantMap &map)
{
    ContactItem item;
    item.mid = map["mid"].toString();
    item.display_name = map["display_name"].toString();
    item.thumbnail_url = map["thumbnail_url"].toString();
    item.picture_path = map["picture_path"].toString();
    item.status_message = map["status_message"].toString();
    item.type = map["type"].toString();
    item.status = map["status"].toString();
    item.relation = map["relation"].toInt();
    return item;
}

QList<ContactItem> ContactItem::FromJsonArray(const QJsonArray &contacts)
{
   QList<ContactItem> result;
   for(const auto& json_contact : contacts) {
       ContactItem item;
       item.mid = json_contact["mid"].toString();
       item.thumbnail_url = json_contact["thumbnailUrl"].toString();
       item.picture_path = json_contact["picturePath"].toString();
       item.display_name = json_contact["displayName"].toString();
       item.status_message = json_contact["statusMessage"].toString();
       item.status = json_contact["status"].toString();
       item.type = json_contact["type"].toString();
       item.relation = json_contact["relation"].toString();
       result.append(std::move(item));
   }
   return result;
}
QList<GroupItem> GroupItem::FromJsonArray(const QJsonArray &groups)
{
   QList<GroupItem> result;
   for(const auto& json_group : groups) {
       GroupItem item;
       item.mid = json_group["chatMid"].toString();
       item.picture_path = json_group["picturePath"].toString();
       item.display_name = json_group["chatName"].toString();

       QJsonObject group_extra = json_group["groupExtra"].toObject();
       item.creator = group_extra["creator"].toString();
       item.ticket =  group_extra["invitationUrl"].toString();
       item.invitess.append(group_extra["inviteeMids"].toObject().keys());
       item.members.append(group_extra["memberMids"].toObject().keys());
       item.join_by_ticket = group_extra["JoinByTicket"].toBool();

       result.append(std::move(item));
   }
   return result;
}

QVariantMap GroupItem::ToMap()
{
  QVariantMap map;
  map["mid"] = mid;
  map["display_name"] = display_name;
  map["picture_path"] = picture_path;
  map["creator"] = creator;
  map["ticket"] = ticket;
  map["members"] = ToList(members);
  map["invitess"] = ToList(invitess);
  map["join_by_ticket"] = join_by_ticket;
  return map;
}

SessionItem SessionItem::FromContactMap(const QVariantMap &map)
{
   SessionItem session;
   session.id = map["id"].toString();
   session.display_name = map["display_name"].toString();
   session.avatar_url = map["avatar_url"].toString();
   session.status_msg = map["status_message"].toString();
   return session;
}

QVariantMap InvitedIntoGroupItem::ToMap()
{
  QVariantMap map = group_item.ToMap();
  map["status"] = status;
  map["invitee"] = invitee;
  map["timestamp"] = timestamp;

  return map;
}

MidType GetMidType(const QString &mid)
{
 if(mid.startsWith("u")){
   return MidTypeUser;
 } else if(mid.startsWith("r")) {
   return MidTypeRoom;
 } else if(mid.startsWith("c")) {
   return MidTypeGroup;
 }
 return MidTypeUnknow;
}


QByteArray EventItemData::ToByteArray() const
{
  QBuffer data;
  data.open(QIODevice::WriteOnly);
  QDataStream stream(&data);
  stream << id << content << sender_id << isMe << size << title <<type << timestamp;
  return data.data();
}

EventItemData EventItemData::FromByteArray(const QByteArray &data)
{
  EventItemData event;
  QDataStream stream(data);
  stream >> event.id >> event.content >> event.sender_id >> event.isMe >> event.size >> event.title >> event.type >> event.timestamp;
  return event;
}

EventItemData EventItemData::FromJsonObject(const QJsonObject &message)
{
  EventItemData event;
  event.id = message["id"].toString();
  event.sender_id = message["from"].toString();
  const auto& contact = LineEngine::Instance()->GetContact(event.sender_id);
  event.sender_name = contact.display_name;
  event.avatar_url = contact.picture_path;
  event.isMe = event.sender_id == LineEngine::Instance()->GetMyMid();
  event.content = message["text"].toString();
  event.timestamp = message["timestamp"].toDouble() / 1000;
  QString content_type = message["contentType"].toString();

  QJsonObject contentMetadata = message["contentMetadata"].toObject();
  event.size = contentMetadata["FILE_SIZE"].toString().toInt();

  if(content_type == "NONE") {
    //可能包含联系人
    QString org_contp = contentMetadata["ORGCONTP"].toString();
    if(org_contp == "CONTACT") {
       event.type = EnumDefine::EventTypeVcard;
       event.title = event.content;
       event.source = contentMetadata["vCard"].toString();
    } else {
        event.type = EnumDefine::EventTypeText;
    }
  } else if (content_type == "CONTACT") {
      event.type = EnumDefine::EventTypeContact;
      event.title = contentMetadata["displayName"].toString();
      event.source = contentMetadata["mid"].toString();
  } else if (content_type == "FILE") {
      event.type = EnumDefine::EventTypeFile;
      event.title = contentMetadata["FILE_NAME"].toString();
  } else if (content_type == "IMAGE") {
      event.type = EnumDefine::EventTypeImage;
  } else if (content_type == "VIDEO") {
      event.type = EnumDefine::EventTypeVideo;
  } else if (content_type == "AUDIO") {
      event.type = EnumDefine::EventTypeAudio;
  }
  event.status = EnumDefine::EventItemStatusSuccess;
  return event;
}
