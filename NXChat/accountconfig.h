#ifndef ACCOUNTCONFIG_H
#define ACCOUNTCONFIG_H
#include <QString>
#include <QSqlDatabase>
#include "modelitem.h"

class AccountConfig
{
public:
  AccountConfig(const QString& name);  
  void UpdateContacts(const QList<ContactItem>& contacts);
  void UpdateContactName(const QString& mid, const QString& name);
  bool GetContact(const QString& mid, ContactItem& item);
  QList<ContactItem> GetContacts();


  //群操作
  void UpdateGroups(const QList<GroupItem>& groups);
  void UpdateGroupName(const QString& group_id, const QString& name);
  bool GetGroup(const QString& group_id, GroupItem& item);
  void DeleteGroup(const QString& group_id);
  QList<GroupItem> GetGroups();

  //邀请
  void UpdateInvitedGroups(const InvitedIntoGroupItem& invite_group);
  void UpdateInvitedGroup(const QString& group_id, int status);
  void DeleteInvitedGroup(const QString& group_id);
  QList<InvitedIntoGroupItem> GetInvitedGroups();

  //session 表操作
  QList<SessionItem> GetSessions();
  void UpdateSession(const SessionItem& session);
  void DeleteSession(const QString& id);

  //聊天消息
  void CreateChatTable(const QString& session_id);
  void InsertChatMessage(const QString& session_id, const EventItemData& event);
  void UpdateChatMessageStatus(const QString& session_id, const QString& msg_id, EnumDefine::EventItemStatus status);
  void UpdateChatMessage(const QString& session_id, const QString& msg_id, const QVariant& value, AbstractEventRoleName role);


  QList<EventItemData> GetChatMessages(const QString& session_id, int offset, int count);

  //setting
  template<class T>
  void SetSetting(const QString&key, const T& value) {
      InterSetSetting(key, QString("%1").arg(value));
  }


  bool GetBoolSetting(const QString&key) {
      return InterGetSetting(key).toInt();
  }

  int GetIntSetting(const QString&key) {
      return InterGetSetting(key).toInt();
  }

  void SetSetting(const QString& key, const QString& value);
  QString GetSetting(const QString& key);

  const QString& GetImageCacheDir();
  const QString& GetVideoCacheDir();
  const QString&GetAudioCacheDir();
  const QString&GetFileCacheDir();

private:
  void InterSetSetting(const QString& key, const QString& value);
  QString InterGetSetting(const QString& key);

  void CreateTable();



private:
  QSqlDatabase db_;
  QString image_cache_dir_;
  QString video_cache_dir_;
  QString audio_cache_dir_;
  QString file_cache_dir_;
};

#endif // ACCOUNTCONFIG_H
