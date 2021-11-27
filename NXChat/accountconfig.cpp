#include "accountconfig.h"
#include <QApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBuffer>

AccountConfig::AccountConfig(const QString& name)
{
  QString exePath = QCoreApplication::applicationFilePath();
  QDir dir(exePath);
  dir.cdUp();

  dir.mkpath("config");

  QDir downloadDir(dir.absoluteFilePath("download"));

  downloadDir.mkpath("images");
  downloadDir.mkpath("videos");
  downloadDir.mkdir("audios");
  downloadDir.mkdir("files");

  image_cache_dir_ = downloadDir.absoluteFilePath("images");
  video_cache_dir_ = downloadDir.absoluteFilePath("videos");
  audio_cache_dir_= downloadDir.absoluteFilePath("audios");
  file_cache_dir_ = downloadDir.absoluteFilePath("files");


  QDir config_dir =QDir(dir.absoluteFilePath("config"));
  QString global_db = config_dir.absoluteFilePath(name);
  bool db_exist = QFile::exists(global_db);

  db_ = QSqlDatabase::addDatabase("QSQLITE", name);
  db_.setDatabaseName(global_db);
  if (!db_.open()) {
    QMessageBox::warning(0, QString::fromStdWString(L"Database error"),
                         db_.lastError().text());
    return;
  }
  if(!db_exist) {
      CreateTable();
  }
}

void AccountConfig::CreateTable()
{
    {
        //普通设置
        QSqlQuery query(db_);
        query.exec(
            "CREATE TABLE IF NOT EXISTS setting (key text primary  key, value text);");
    }
    {
      //创建联系人
      QSqlQuery query(db_);
      query.exec(
          "CREATE TABLE IF NOT EXISTS contact (mid text primary  key, display_name text, thumbnailUrl text, picturePath text, type text, status text, status_message text, relation text);");
    }
    {
      //创建群
      QSqlQuery query(db_);
      query.exec(
          "CREATE TABLE IF NOT EXISTS groups (mid text primary  key, display_name text, picturePath text, creator text, invitationUrl text, members BLOB, invitess BLOB, join_byticket bool);");
    }
    {
        //被邀请入群
        QSqlQuery query(db_);
        query.exec(
            "CREATE TABLE IF NOT EXISTS invited_into_groups (mid text primary  key, display_name text, picturePath text, creator text, invitationUrl text, members BLOB, invitess BLOB, join_byticket bool, status int, invitee text, timestamp long);");

    }
    {
      //会话
      QSqlQuery query(db_);
      query.exec(
          "CREATE TABLE IF NOT EXISTS sessions (mid text primary  key, display_name text, picturePath text, type int, status_message text, timestamp int);");
    }
}

void AccountConfig::UpdateContacts(const QList<ContactItem> &contacts)
{
    db_.transaction();
    for(auto& item : contacts) {
        QSqlQuery query(db_);
        query.prepare("INSERT OR REPLACE INTO contact (mid, display_name,thumbnailUrl,picturePath,type,status, status_message, relation) values(?, ?, ?,?,?,?,?,?)");
        query.addBindValue(item.mid);
        query.addBindValue(item.display_name);
        query.addBindValue(item.thumbnail_url);
        query.addBindValue(item.picture_path);
        query.addBindValue(item.type);
        query.addBindValue(item.status);
        query.addBindValue(item.status_message);
        query.addBindValue(item.relation);
        query.exec();
    }

    db_.commit();
}

void AccountConfig::UpdateContactName(const QString &mid, const QString &name)
{
  QSqlQuery query(db_);
  query.prepare("update contact set display_name=? where mid = ?");
  query.addBindValue(name);
  query.addBindValue(mid);
  query.exec();
}

static void ParseContact(QSqlQuery& query, ContactItem& item) {
  item.mid = query.value("mid").toString();
  item.display_name = query.value("display_name").toString();
  item.thumbnail_url = query.value("thumbnailUrl").toString();
  item.picture_path = query.value("picturePath").toString();
  item.type = query.value("type").toString();
  item.status = query.value("status").toString();
  item.status_message = query.value("status_message").toString();
  item.relation = query.value("relation").toString();
}

static void ParseGroup(const QSqlQuery& query, GroupItem& item) {
  //mid, display_name,picturePath,creator,invitationUrl,members,invitess
  item.mid = query.value("mid").toString();
  item.display_name = query.value("display_name").toString();
  item.picture_path = query.value("picturePath").toString();
  item.creator = query.value("creator").toString();
  item.ticket = query.value("invitationUrl").toString();
  {
      QDataStream stream(query.value("members").toByteArray());
      stream >> item.members;
  }
  {
      QDataStream stream(query.value("invitess").toByteArray());
      stream >> item.invitess;
  }
}

bool AccountConfig::GetContact(const QString &mid, ContactItem& item)
{
  QSqlQuery query(db_);
  query.prepare("select * from contact where mid = ?");
  query.addBindValue(mid);
  query.exec();
  if(query.next()) {
    ParseContact(query, item);
    return true;
  }
  return false;
}

QList<ContactItem> AccountConfig::GetContacts()
{
    QList<ContactItem> result;
    QSqlQuery query(db_);
    query.prepare("select * from contact;");
    query.exec();
    while (query.next()) {
        ContactItem item;
        ParseContact(query, item);
        result.append(std::move(item));
    }

    return result;
}

static void BindGroup(QSqlQuery& query, const GroupItem& item) {
  query.addBindValue(item.mid);
  query.addBindValue(item.display_name);
  query.addBindValue(item.picture_path);
  query.addBindValue(item.creator);
  query.addBindValue(item.ticket);

  {
      QBuffer data;
      data.open(QIODevice::WriteOnly);
      QDataStream stream(&data);
      stream << item.members;
      query.addBindValue(data.data());
  }

  {
      QBuffer data;
      data.open(QIODevice::WriteOnly);
      QDataStream stream(&data);
      stream << item.invitess;
      query.addBindValue(data.data());
  }
  query.addBindValue(item.join_by_ticket);
}

void AccountConfig::UpdateGroups(const QList<GroupItem> &groups)
{
    db_.transaction();
    for(auto& item : groups) {
        QSqlQuery query(db_);
        query.prepare("INSERT OR REPLACE INTO groups (mid, display_name,picturePath,creator,invitationUrl,members,invitess,join_byticket) values(?, ?, ?,?,?,?,?,?)");
        BindGroup(query, item);
        query.exec();
    }

    db_.commit();
}

void AccountConfig::UpdateGroupName(const QString &group_id, const QString &name)
{
    QSqlQuery query(db_);
    query.prepare("update groups set display_name=? where mid = ?");
    query.addBindValue(name);
    query.addBindValue(group_id);
    query.exec();
}

bool AccountConfig::GetGroup(const QString &group_id, GroupItem &item)
{
  QSqlQuery query(db_);
  query.prepare("select * from groups where mid = ?");
  query.addBindValue(group_id);
  query.exec();
  if(query.next()) {
    ParseGroup(query, item);
    return true;
  }
  return false;
}

void AccountConfig::DeleteGroup(const QString &group_id)
{
    QSqlQuery query(db_);
    query.prepare("delete from groups where mid = ?");
    query.addBindValue(group_id);
    query.exec();
}

QList<GroupItem> AccountConfig::GetGroups()
{
    QList<GroupItem> result;
    QSqlQuery query(db_);
    query.prepare("select * from groups;");
    query.exec();
    while (query.next()) {
        GroupItem item;
        ParseGroup(query, item);
        result.append(std::move(item));
    }

    return result;
}

void AccountConfig::UpdateInvitedGroups(const InvitedIntoGroupItem &invite_group)
{
  QSqlQuery query(db_);
  query.prepare("INSERT OR REPLACE INTO invited_into_groups (mid, display_name,picturePath,creator,invitationUrl,members,invitess,join_byticket,status, invitee,timestamp) values(?, ?, ?,?,?,?,?,?,?,?,?)");
  BindGroup(query, invite_group.group_item);
  query.addBindValue(invite_group.status);
  query.addBindValue(invite_group.invitee);
  query.addBindValue(invite_group.timestamp);
  query.exec();
}

void AccountConfig::UpdateInvitedGroup(const QString &group_id, int status)
{
    QSqlQuery query(db_);
    query.prepare("update invited_into_groups set status=? where mid = ?");
    query.addBindValue(status);
    query.addBindValue(group_id);
    query.exec();
}

void AccountConfig::DeleteInvitedGroup(const QString &group_id)
{
    QSqlQuery query(db_);
    query.prepare("delete from invited_into_groups where mid = ?");
    query.addBindValue(group_id);
    query.exec();
}

QList<InvitedIntoGroupItem> AccountConfig::GetInvitedGroups()
{
  QList<InvitedIntoGroupItem> result;
  QSqlQuery query(db_);
  query.prepare("select * from invited_into_groups order by timestamp DESC;");
  query.exec();
  while (query.next()) {
      InvitedIntoGroupItem item;
      //mid, display_name,picturePath,creator,invitationUrl,members,invitess, status, invitee,timestamp
      ParseGroup(query, item.group_item);
      item.status = query.value("status").toInt();
      item.invitee = query.value("invitee").toString();
      item.timestamp = query.value("timestamp").toInt();
      result.append(std::move(item));
  }

  return result;
}

QList<SessionItem> AccountConfig::GetSessions()
{
  QList<SessionItem> result;
  QSqlQuery query(db_);
  query.prepare("select * from sessions order by timestamp DESC;");
  query.exec();
  while (query.next()) {
      //mid text primary  key, display_name text, picturePath text, type int
      SessionItem item;
      item.id = query.value("mid").toString();
      item.display_name = query.value("display_name").toString();
      item.avatar_url = query.value("picturePath").toString();
      item.type = (EnumDefine::SessionType)query.value("type").toInt();
      item.status_msg = query.value("status_message").toString();
      item.timestamp = query.value("timestamp").toInt();

      result.append(std::move(item));
  }

  return result;
}

void AccountConfig::UpdateSession(const SessionItem &session)
{
  QSqlQuery query(db_);
  query.prepare("INSERT OR REPLACE INTO sessions (mid, display_name,picturePath,type,status_message,timestamp) values(?, ?, ?,?,?,?);");
  query.addBindValue(session.id);
  query.addBindValue(session.display_name);
  query.addBindValue(session.avatar_url);
  query.addBindValue(session.type);
  query.addBindValue(session.status_msg);
  query.addBindValue(session.timestamp);
  query.exec();
}

void AccountConfig::DeleteSession(const QString &id)
{
  {
    QSqlQuery query(db_);
    query.prepare("DELETE from sessions where mid=?;");
    query.addBindValue(id);
    query.exec();
  }

  {
    QSqlQuery query(db_);
    QString sql = QString("DROP TABLE %1;").arg(id);
    query.exec(std::move(sql));
  }
}

void AccountConfig::CreateChatTable(const QString &session_id)
{
  QSqlQuery query(db_);
  QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (id text primary  key, value BLOB, source text, status int, timestamp int);").arg(session_id);
  query.exec(std::move(sql));
}

void AccountConfig::InsertChatMessage(const QString &session_id, const EventItemData &event)
{
  QSqlQuery query(db_);
  QString sql = "INSERT OR REPLACE INTO %1 (id, value, source, status, timestamp) values(?, ?, ?, ?, ?);";
  query.prepare(sql.arg(session_id));
  query.addBindValue(event.id);

  query.addBindValue(event.ToByteArray());

  query.addBindValue(event.source);
  query.addBindValue(event.status);

  query.addBindValue(event.timestamp);
  query.exec();
}

void AccountConfig::UpdateChatMessageStatus(const QString &session_id, const QString &msg_id, EnumDefine::EventItemStatus status)
{
    QSqlQuery query(db_);
    QString sql = "Update %1 set status = ? where id = ?;";
    query.prepare(sql.arg(session_id));
    query.addBindValue(status);
    query.addBindValue(msg_id);
    query.exec();
}

void AccountConfig::UpdateChatMessage(const QString &session_id, const QString &msg_id, const QVariant& value, AbstractEventRoleName role)
{
    QSqlQuery query(db_);
    QString sql;
    switch (role) {
    case AbstractEventRoleName::Source :{
        sql = "Update %1 set source = ? where id = ?;";
      }
        break;
    case AbstractEventRoleName::Status :{
        sql = "Update %1 set status = ? where id = ?;";
      }
        break;
    }

    query.prepare(sql.arg(session_id));
    switch (role) {
    case AbstractEventRoleName::Source :{
        query.addBindValue(value.toString());
      }
        break;
    case AbstractEventRoleName::Status :{
        query.addBindValue(value.toInt());
      }
        break;
    }
    query.addBindValue(msg_id);
    query.exec();
}

QList<EventItemData> AccountConfig::GetChatMessages(const QString& session_id, int offset, int count)
{
  QList<EventItemData> results;
  QSqlQuery query(db_);
  QString sql = QString("select * from %1 order by timestamp DESC;").arg(session_id);
  query.prepare(sql);
  query.exec();
  while (query.next()) {
      //mid text primary  key, display_name text, picturePath text, type int
      EventItemData event = EventItemData::FromByteArray(query.value("value").toByteArray());
      event.source = query.value("source").toString();
      event.status = (EnumDefine::EventItemStatus)query.value("status").toInt();
      if(event.status == EnumDefine::EventItemStatusSending) {
          event.status = EnumDefine::EventItemStatusFailed;
      }
      results.append(std::move(event));
  }
  return results;
}

void AccountConfig::SetSetting(const QString &key, const QString &value)
{
    InterSetSetting(key, value);
}

QString AccountConfig::GetSetting(const QString &key)
{
    return InterGetSetting(key);
}

const QString & AccountConfig::GetImageCacheDir()
{
    return image_cache_dir_;
}

const QString &AccountConfig::GetVideoCacheDir()
{
    return video_cache_dir_;
}

const QString &AccountConfig::GetAudioCacheDir()
{
    return audio_cache_dir_;
}

const QString &AccountConfig::GetFileCacheDir()
{
    return file_cache_dir_;
}

void AccountConfig::InterSetSetting(const QString &key, const QString &value)
{
    QSqlQuery query(db_);
    query.prepare("INSERT OR REPLACE INTO setting (key, value) values(?, ?);");
    query.addBindValue(key);
    query.addBindValue(value);

    query.exec();
}

QString AccountConfig::InterGetSetting(const QString &key)
{
    QSqlQuery query(db_);
    query.prepare("select * from setting where key = ?;");
    query.addBindValue(key);
    query.exec();
    if (query.next()) {
        return query.value("value").toString();
    }
    return "";
}
