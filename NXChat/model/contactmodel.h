#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>
#include "modelitem.h"

class ContactModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ContactModel(QObject *parent = nullptr);

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

    void SetContacts(QList<ContactItem>&& contacts);



private:
    enum AbstractContactRoleName {
        Id,
        DisplayName,
        ThumbnailUrl,
        PicturePath,
        Status,
        StatusMessage,
        Type
    };

    QList<ContactItem> data_;
};

#endif // CONTACTMODEL_H
