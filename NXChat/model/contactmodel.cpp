#include "contactmodel.h"

ContactModel::ContactModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return data_.size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
       case AbstractContactRoleName::DisplayName: {
            return data_[index.row()].display_name;
       }
       case AbstractContactRoleName::Id: {
            return data_[index.row()].mid;
       }
        case AbstractContactRoleName::ThumbnailUrl: {
             return data_[index.row()].thumbnail_url;
        }
        case AbstractContactRoleName::PicturePath: {
             return data_[index.row()].picture_path;
        }
        case AbstractContactRoleName::StatusMessage: {
             return data_[index.row()].status_message;
        }
        case AbstractContactRoleName::Status: {
             return data_[index.row()].status;
        }
        case AbstractContactRoleName::Type: {
             return data_[index.row()].type;
        }
    }
    return QVariant();
}

bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool ContactModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool ContactModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> ContactModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(AbstractContactRoleName::DisplayName, "display_name");
    roles.insert(AbstractContactRoleName::Id, "id");
    roles.insert(AbstractContactRoleName::ThumbnailUrl, "thumbnail_url");
    roles.insert(AbstractContactRoleName::PicturePath, "picture_path");
    roles.insert(AbstractContactRoleName::Status, "status");
    roles.insert(AbstractContactRoleName::StatusMessage, "status_message");
    roles.insert(AbstractContactRoleName::Type, "type");
    return roles;
}

void ContactModel::SetContacts(QList<ContactItem> &&contacts)
{
    data_ = std::move(contacts);
    //排序
    std::sort(data_.begin(), data_.end());
}
