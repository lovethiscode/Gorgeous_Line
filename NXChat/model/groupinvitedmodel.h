#ifndef GROUPINVITEDMODEL_H
#define GROUPINVITEDMODEL_H

#include <QAbstractListModel>
#include "modelitem.h"

class GroupInvitedModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit GroupInvitedModel(QObject *parent = nullptr);
  void SetData(QList<InvitedIntoGroupItem>&& data);

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  void RemoveItem(const QString& mid);
  void SetStatus(const QString& mid, int status);
  void UpdateInvitedGroups(const InvitedIntoGroupItem& item);


  QHash<int,QByteArray> roleNames() const override;
  enum AbstractGroupInvitedRoleName{
    DisplayName,
    MID,
    PicturePath,
    Status,
    TimeStamp
  };

private:
  QList<InvitedIntoGroupItem> data_;
};

#endif // GROUPINVITEDMODEL_H
