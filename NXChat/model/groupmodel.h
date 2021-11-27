#pragma once
#include <QAbstractListModel>
#include "modelitem.h"

class GroupModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GroupModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    QHash<int,QByteArray> roleNames() const override;

    void SetGroups(QList<GroupItem>&& groups);

private:
  enum AbstractGroupRoleName {
      Id,
      DisplayName,
      PicturePath,
      Creator,
      Ticket,
      Members,
  };

  QList<GroupItem> data_;
};

