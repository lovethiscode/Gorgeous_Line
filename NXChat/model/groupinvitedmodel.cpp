#include "groupinvitedmodel.h"

GroupInvitedModel::GroupInvitedModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

void GroupInvitedModel::SetData(QList<InvitedIntoGroupItem> &&data)
{
  data_ = std::move(data);
}

int GroupInvitedModel::rowCount(const QModelIndex &parent) const
{
  // For list models only the root node (an invalid parent) should return the list's size. For all
  // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
  if (parent.isValid())
    return 0;

  // FIXME: Implement me!
  return data_.size();
}

QVariant GroupInvitedModel::data(const QModelIndex &index, int role) const
{
  switch (role) {
     case AbstractGroupInvitedRoleName::DisplayName: {
          return data_[index.row()].group_item.display_name;
     }
     case AbstractGroupInvitedRoleName::MID: {
          return data_[index.row()].group_item.mid;
     }

      case AbstractGroupInvitedRoleName::PicturePath: {
           return data_[index.row()].group_item.picture_path;
      }

      case AbstractGroupInvitedRoleName::Status: {
           return data_[index.row()].status;
      }
      case AbstractGroupInvitedRoleName::TimeStamp: {
          return QDateTime::fromSecsSinceEpoch(data_[index.row()].timestamp).toString();
      }
  }
  return QVariant();
}

bool GroupInvitedModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (data(index, role) != value) {
    // FIXME: Implement me!
    switch (role) {
      case AbstractGroupInvitedRoleName::Status :{
        data_[(index.row())].status = value.toInt();
        break;
      }
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

void GroupInvitedModel::RemoveItem(const QString &mid)
{
  InvitedIntoGroupItem item;
  item.group_item.mid = mid;
  int row = data_.indexOf(item);
  if(-1 == row)
  {
    return;
  }
  beginRemoveRows(QModelIndex(), row, row);
  data_.removeAt(row);
  endRemoveRows();
}

void GroupInvitedModel::SetStatus(const QString &mid, int status)
{
  InvitedIntoGroupItem item;
  item.group_item.mid = mid;
  int row = data_.indexOf(item);
  if(-1 == row)
  {
    return;
  }
  setData(QAbstractItemModel::createIndex(row, 0), status, AbstractGroupInvitedRoleName::Status);
}

void GroupInvitedModel::UpdateInvitedGroups(const InvitedIntoGroupItem &item)
{
  int row = data_.indexOf(item);
  if(-1 == row)
  {
    //没有就插入一行
    beginInsertRows(QModelIndex(), 0, 0);
    data_.insert(0, item);
    endInsertRows();

    return;
  }
  data_[row] = item;

  QModelIndex index = QAbstractItemModel::createIndex(row, 0);
  emit dataChanged(index, index, QVector<int>() << DisplayName << PicturePath << Status);
}

QHash<int, QByteArray> GroupInvitedModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles.insert(AbstractGroupInvitedRoleName::DisplayName, "display_name");
  roles.insert(AbstractGroupInvitedRoleName::MID, "mid");
  roles.insert(AbstractGroupInvitedRoleName::PicturePath, "picture_path");
  roles.insert(AbstractGroupInvitedRoleName::Status, "status");
  roles.insert(AbstractGroupInvitedRoleName::TimeStamp, "timestamp");
  return roles;
}
