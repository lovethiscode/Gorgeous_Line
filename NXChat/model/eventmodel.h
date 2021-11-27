#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include "modelitem.h"

class EventModel : public QAbstractListModel
{
    Q_OBJECT 
public:
  EventModel() {}
    explicit EventModel(const QString& session_id, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    void AddEvent(int row, const EventItemData &event_item);
    void UpdateEventStatus(const QString &msg_id, int status);
    void UpdateEvent(const QString &msg_id, const QVariant& value, AbstractEventRoleName role);

    void SetEvents(QList<EventItemData> && events);



private:
    QString session_id_;

    QList<EventItemData> data_;
};

#endif // EVENTMODEL_H
