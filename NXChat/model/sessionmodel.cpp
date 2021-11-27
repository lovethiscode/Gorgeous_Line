#include "sessionmodel.h"
#include "lineengine.h"

SessionModel::SessionModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int SessionModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return data_.length();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
       case AbstractSessionRoleName::DisplayName: {
            return data_[index.row()].display_name;
       }
       case AbstractSessionRoleName::Id: {
            return data_[index.row()].id;
       }
        case AbstractSessionRoleName::Presence: {
             return data_[index.row()].presence;
        }
        case AbstractSessionRoleName::AvatarUrl: {
             return data_[index.row()].avatar_url;
        }
        case AbstractSessionRoleName::StatusMsg: {
             return data_[index.row()].status_msg;
        }
        case AbstractSessionRoleName::Type: {
             return data_[index.row()].type;
        }
        case AbstractSessionRoleName::TotalUnread: {
        return data_[index.row()].total_unread;
   }
    }
    return QVariant();
}

bool SessionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  switch (role) {
    case AbstractSessionRoleName::TotalUnread: {
      data_[index.row()].total_unread = value.toInt();
    }
      break;
  }
  return 0;
}

QHash<int, QByteArray> SessionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(AbstractSessionRoleName::DisplayName, "display_name");
    roles.insert(AbstractSessionRoleName::Id, "id");
    roles.insert(AbstractSessionRoleName::Presence, "presence");
    roles.insert(AbstractSessionRoleName::AvatarUrl, "avatar_url");
    roles.insert(AbstractSessionRoleName::StatusMsg, "status_msg");
    roles.insert(AbstractSessionRoleName::Type, "type");
    roles.insert(AbstractSessionRoleName::TotalUnread, "total_unread");
    return roles;
}

void SessionModel::RemoveSession(const QString& session_id)
{
  SessionItem session;
  session.id = session_id;
  int row = data_.indexOf(session);
  if(-1 == row) {
    return;
  }
  beginRemoveRows(QModelIndex(), row, row);
  data_.removeAt(row);
  endRemoveRows();
  //删除对应的event
  auto iter = event_model_map_.find(session_id);
  if(iter != event_model_map_.end()){
    event_model_map_.erase(iter);
  }
}

int SessionModel::AddSession(int row, const SessionItem& session)
{
    int result_row = data_.indexOf(session);
    if(-1 != result_row){
        return result_row;
    }
    //创建数据表
    LineEngine::Instance()->account_config_->CreateChatTable(session.id);

    beginInsertRows(QModelIndex(), row, row);
    data_.insert(row, session);
    endInsertRows(); 
    return row;
}

bool SessionModel::HasSession(const QString &session_id)
{
  SessionItem item;
  item.id = session_id;
  return HasSession(item);
}

bool SessionModel::HasSession(const SessionItem &session)
{
  return data_.indexOf(session) != -1;
}

void SessionModel::UpdateSession(SessionItem &session)
{
  int row = data_.indexOf(session);
  if(-1 == row) {
    return;
  }
  session.timestamp = data_[row].timestamp;
  data_[row] = session;

  QModelIndex index = QAbstractItemModel::createIndex(row, 0);
  emit dataChanged(index, index, QVector<int>() << Id << DisplayName << Presence << AvatarUrl << StatusMsg << Type);
}

void SessionModel::AddChatMessage(const QString &session_id, const EventItemData &event_item)
{
  EventModel* event_model = getEventModel(session_id);
  event_model->AddEvent(0, event_item);

  SessionItem session;
  session.id = session_id;
  int row = data_.indexOf(session);
  if(-1 == row) {
    return;
  }
  if(current_index_ == row) {
    return;
  }

  data_[row].total_unread ++;
  QModelIndex index = QAbstractItemModel::createIndex(row, 0);
  emit dataChanged(index, index, QVector<int>() << TotalUnread);
}

void SessionModel::UpdateChatMessage(const QString &session_id, const QString &msg_id, const QVariant& value, AbstractEventRoleName role)
{
    EventModel* event_model = getEventModel(session_id);
    event_model->UpdateEvent(msg_id, value, role);
}

EventModel *SessionModel::getEventModel(const QString& session_id)
{
  auto iter = event_model_map_.find(session_id);
  if(iter != event_model_map_.end()){
    return iter.value();
  }

  auto event_model  = new EventModel(session_id);
  //初始化聊天数据
  auto events = LineEngine::Instance()->account_config_->GetChatMessages(session_id, 0, 20);
  for(auto& event : events) {
    const auto& contact = LineEngine::Instance()->GetContact(event.sender_id);
    event.sender_name = contact.display_name;
    event.avatar_url = contact.picture_path;
  }

  event_model->SetEvents(std::move(events));
  event_model_map_[session_id] = event_model;
  return event_model;
}

void SessionModel::setCurrentIndex(int current_index)
{
  if(current_index_ != -1) {
    //将上一个设置位0
    data_[current_index_].total_unread = 0;
    QModelIndex index = QAbstractItemModel::createIndex(current_index_, 0);
    emit dataChanged(index, index, QVector<int>() << TotalUnread);

  }
  current_index_ = current_index;
}

void SessionModel::SetSessions(QList<SessionItem> &&data)
{
    if(data.empty()) {
        return;
    }
    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    data_ = std::move(data);
    endInsertRows();
}
