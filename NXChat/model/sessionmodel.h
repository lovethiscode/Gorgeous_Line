#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>
#include "modelitem.h"
#include "eventmodel.h"

class SessionModel : public QAbstractListModel
{
    Q_OBJECT
  Q_PROPERTY(int current_index MEMBER current_index_ WRITE setCurrentIndex)

public:
    explicit SessionModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QHash<int,QByteArray> roleNames() const override;

    //Q_INVOKABLE QVariantMap find(const QString& id);

    void RemoveSession(const QString& session_id);
    int AddSession(int row, const SessionItem& session);
    bool HasSession(const QString& session_id);
    bool HasSession(const SessionItem& session);
    void UpdateSession(SessionItem& session);

    void AddChatMessage(const QString& session_id, const EventItemData &event_item);

    void UpdateChatMessage(const QString& session_id, const QString& msg_id, const QVariant& value, AbstractEventRoleName role);

    Q_INVOKABLE EventModel* getEventModel(const QString& session_id);
    void setCurrentIndex(int current_index);

    void SetSessions(QList<SessionItem>&& data);

public slots:


private:
    enum AbstractSessionRoleName {
        Id,
        DisplayName,
        Presence,
        AvatarUrl,
        StatusMsg,
        Type,
        TotalUnread
    };

    QList<SessionItem> data_;
    QMap<QString, EventModel*> event_model_map_;
    int current_index_ = -1;
};

#endif // CONTACTITEM_H
