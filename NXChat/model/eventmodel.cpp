#include "eventmodel.h"
#include "lineengine.h"

EventModel::EventModel(const QString& session_id, QObject *parent)
    : QAbstractListModel(parent),
      session_id_(session_id)
{
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return data_.length();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
       case AbstractEventRoleName::Id: {
            return data_[index.row()].id;
       }
       case AbstractEventRoleName::SenderId: {
            return data_[index.row()].sender_id;
       }

       case AbstractEventRoleName::SenderName: {
             return data_[index.row()].sender_name;
       }
       case AbstractEventRoleName::Content: {
             return data_[index.row()].content;
       }
       case AbstractEventRoleName::AvatarUrl: {
             return data_[index.row()].avatar_url;
       }
       case AbstractEventRoleName::IsMe: {
             return data_[index.row()].isMe;
       }
       case AbstractEventRoleName::Date: {
             return QDateTime::fromSecsSinceEpoch(data_[index.row()].timestamp).toString("hh:mm");
       }
        case AbstractEventRoleName::Type: {
            return data_[index.row()].type;
        }
        case AbstractEventRoleName::Source: {
            if(data_[index.row()].source.isEmpty()) {
                return "";
            }
            return "file:///" + data_[index.row()].source;
        }
        case AbstractEventRoleName::Title: {
            return data_[index.row()].title;
        }
        case AbstractEventRoleName::Size: {
            return data_[index.row()].size;
        }
        case AbstractEventRoleName::Status: {
          return (int)data_[index.row()].status;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(AbstractEventRoleName::Id, "id");
    roles.insert(AbstractEventRoleName::SenderId, "sender_id");
    roles.insert(AbstractEventRoleName::SenderName, "sender_name");
    roles.insert(AbstractEventRoleName::Content, "content");
    roles.insert(AbstractEventRoleName::IsMe, "isMe");
    roles.insert(AbstractEventRoleName::AvatarUrl, "avatar_url");   
    roles.insert(AbstractEventRoleName::Date, "date");
    roles.insert(AbstractEventRoleName::Type, "type");
    roles.insert(AbstractEventRoleName::Source, "source");
    roles.insert(AbstractEventRoleName::Title, "title");
    roles.insert(AbstractEventRoleName::Size, "size");
    roles.insert(AbstractEventRoleName::Status, "status");
    return roles;
}

void EventModel::AddEvent(int row, const EventItemData &event_item)
{
  beginInsertRows(QModelIndex(), row, row);
  data_.insert(row, event_item);
  endInsertRows();
  //保存数据库
  LineEngine::Instance()->account_config_->InsertChatMessage(session_id_, event_item);
}

void EventModel::UpdateEventStatus(const QString &msg_id, int status)
{
    EventItemData item;
    item.id = msg_id;
    int row = data_.indexOf(item);
    if(-1 == row) {
        return;
    }
    QModelIndex index = QAbstractItemModel::createIndex(row, 0);
    data_[row].status = (EnumDefine::EventItemStatus)status;
    emit dataChanged(index, index, QVector<int>() << Status);
    //更新数据库
    LineEngine::Instance()->account_config_->UpdateChatMessageStatus(session_id_, msg_id, (EnumDefine::EventItemStatus)status);
}

void EventModel::UpdateEvent(const QString &msg_id, const QVariant& value, AbstractEventRoleName role)
{
    EventItemData item;
    item.id = msg_id;
    int row = data_.indexOf(item);
    if(-1 == row) {
        return;
    }
    QModelIndex index = QAbstractItemModel::createIndex(row, 0);
    switch (role) {
    case Source: {
        data_[row].source = value.toString();
    }
        break;
    case Status: {
        data_[row].status = (EnumDefine::EventItemStatus)value.toInt();
    }
        break;
    }
    emit dataChanged(index, index, QVector<int>() << role);
    //更新数据库
    LineEngine::Instance()->account_config_->UpdateChatMessage(session_id_, msg_id, value, role);
}

void EventModel::SetEvents(QList<EventItemData> &&events)
{
  data_ = std::move(events);
}
