#include "groupmodel.h"

GroupModel::GroupModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int GroupModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return data_.size();
}

QVariant GroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
       case AbstractGroupRoleName::DisplayName: {
            return data_[index.row()].display_name;
       }
       case AbstractGroupRoleName::Id: {
            return data_[index.row()].mid;
       }
        case AbstractGroupRoleName::PicturePath: {
             return data_[index.row()].picture_path;
        }
        case AbstractGroupRoleName::Creator: {
             return data_[index.row()].creator;
        }
        case AbstractGroupRoleName::Ticket: {
             return data_[index.row()].ticket;
        }
        case AbstractGroupRoleName::Members: {
             return ToList(data_[index.row()].members);
        }
    }
    return QVariant();
}

bool GroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags GroupModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool GroupModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool GroupModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> GroupModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(AbstractGroupRoleName::DisplayName, "display_name");
    roles.insert(AbstractGroupRoleName::Id, "id");
    roles.insert(AbstractGroupRoleName::PicturePath, "picture_path");
    roles.insert(AbstractGroupRoleName::Creator, "creator");
    roles.insert(AbstractGroupRoleName::Ticket, "ticket");
    roles.insert(AbstractGroupRoleName::Members, "members");
    return roles;
}

void GroupModel::SetGroups(QList<GroupItem> &&groups)
{
  data_ = std::move(groups);
  //排序
  std::sort(data_.begin(), data_.end());
}
