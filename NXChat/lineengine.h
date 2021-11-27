#ifndef LINEENGINE_H
#define LINEENGINE_H

#include <QObject>
#include <QtWebSockets>
#include <QTimer>
#include <QMap>
#include "accountconfig.h"
#include "model/sessionmodel.h"
#include "model/contactmodel.h"
#include "model/eventmodel.h"
#include "model/groupinvitedmodel.h"
#include "model/groupmodel.h"
#include "httpuploadmanager.h"

typedef std::function<void(const QString& command, const QJsonObject& json)> CommandDelegate;

class LineEngine : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QVariantMap profile MEMBER profile_ NOTIFY notifyProfileChanged)

    explicit LineEngine(QObject *parent = nullptr);
    ~LineEngine();

  static LineEngine* Instance();

    //左侧session
    Q_INVOKABLE SessionModel* getSessionModel();
    Q_INVOKABLE void deleteSession(int row, const QString& id);

    //联系人
    Q_INVOKABLE ContactModel* getContactModel();
  //群
  Q_INVOKABLE GroupModel *getGroupModel();

    Q_INVOKABLE void login(const QVariantMap& params);
    Q_INVOKABLE int addSession(const QString& mid, int type);

    Q_INVOKABLE void setNickname(const QString& nickname);
    Q_INVOKABLE void setSignature(const QString& signature);
    Q_INVOKABLE void checkUserIdIsValid(const QString& userid);
    Q_INVOKABLE void setUserId(const QString& userid);
    Q_INVOKABLE void getQrcode();
    Q_INVOKABLE void enableSearchByUserid(bool enable);
    Q_INVOKABLE void setBirthday(const QString& year, const QString& day);
    Q_INVOKABLE void acceptGroupInvited(const QString& groupid, bool accept);

    Q_INVOKABLE GroupInvitedModel* getInvitedGroupsModel();
    Q_INVOKABLE void deleteGroupInvited(const QString& group_id);
    Q_INVOKABLE void deleteInvitedGroupsModel();

    Q_INVOKABLE EventModel* getEventModel(const QString& id);

    const QString& GetMyMid();
    const ContactItem &GetContact(const QString& mid);

    Q_INVOKABLE int searchContact(const QString& user_id);


  //发送消息
  Q_INVOKABLE void sendText(const QString& session_id, const QString&text);
  Q_INVOKABLE void reSendText(const QString& session_id, const QString& msg_id, const QString&text);


  Q_INVOKABLE void sendImage(const QString& session_id, const QString&path);
  Q_INVOKABLE void sendImageData(const QString &session_id, const QVariant &data);
  Q_INVOKABLE void reSendImage(const QString& session_id, const QString& msg_id, const QString&path);


  Q_INVOKABLE void sendVideo(const QString& session_id, const QString&path);
  Q_INVOKABLE void reSendVideo(const QString& session_id, const QString& msg_id, const QString&path);

  Q_INVOKABLE void sendAudio(const QString& session_id, const QString&path);
  Q_INVOKABLE void reSendAudio(const QString& session_id, const QString& msg_id, const QString&path);

  Q_INVOKABLE void sendFile(const QString& session_id, const QString&path);
  Q_INVOKABLE void reSendFile(const QString& session_id, const QString& msg_id, const QString&path);

  void DownloadMedia(const QString session_id, const QString& msgid, const QString&path);

  Q_INVOKABLE void downloadMedia(const QString session_id, const QString& msgid, const QString& title);

signals:
    void commandResponse(const QString& command, const QJsonObject& json);
    void notifyProfileChanged();

private:
    void OnConnected();
    void OnDisconnect();
    void OnMessageReceived(const QString& message);
    void OnError(const QAbstractSocket::SocketError& error);
    int SendCommand(QJsonObject& command, CommandDelegate delegate = nullptr);

    //拦截回复
    bool InterceptResponse(const QString& command, const QJsonObject& json);
    //处理登录事件
    void HandleLogin(const QJsonObject& root);
    void HandleProfile(const QJsonObject& root);
    void HandleAllContact(const QJsonObject& root);
    void HandleJoinedGroups(const QJsonObject& root);
    void HandleGetContacts(const QJsonObject& root);
    void HandleGetGroups(const QJsonObject& root);
    void HandleGetQrCode(const QJsonObject& root);
    void HandleSetSignature(const QJsonObject& root);
    void HandleSetNickName(const QJsonObject& root);
    bool HandleNotify(const QJsonObject& root);

    void HandleReceiveMessage(const QJsonObject& message);
    void HandleChatEvent(const QJsonObject& message);
    void HandleReceiveChatMessage(const QJsonObject& message);
    void AddContactSessionIfNeed(const QString& mid);
    void AddGroupSessionIfNeed(const QString& mid);
    void UpdateSessionIfNeed(const QList<ContactItem> &contacts);
    void UpdateSessionIfNeed(const QList<GroupItem> &groups);



    void HandleInviteIntoChat(const QJsonObject& message);
    void HandleCancelInviteIntoChat(const QJsonObject& message);
    void HandleAcceptChatInvitation(const QJsonObject& message);
    void HandleLeaveChat(const QJsonObject& message);


    void HandleUpdateProfile(const QJsonObject& message);



    static QJsonArray ToJsonArray(const QString& json);
    static QJsonObject ToJsonObject(const QString& json);

    void InitSessions();

 public  slots:

private:
    void UploadObject(const QJsonObject& data, UploadDelegate delegate);
    void GetInviteGroupInfo(const InvitedIntoGroupItem& group);
    void SearchGroups(const QJsonArray& ids, CommandDelegate delegate = nullptr);
    void GetContacts(const QJsonArray& ids, CommandDelegate delegate = nullptr);

    void HandleSendMedia(QJsonObject &command, const QString& session_id, const QString& msg_id, const QString& path, EnumDefine::EventType type, bool retry);
    void UploadMedia(const QJsonObject& json, const QString& session_id, const QString& msg_id);
    void AutoDownloadMedia(const QString& session_id, const EventItemData& event);

    void InnerSendText(const QString &session_id, const QString &msg_id, const QString &text);
    QJsonObject GenerateSendImageCommand(const QString &session_id, const QString &msg_id, const QString &path);
    QJsonObject GenerateSendVideoCommand(const QString &session_id, const QString &msg_id, const QString &path);
    QJsonObject GenerateSendAudioCommand(const QString &session_id, const QString &msg_id, const QString &path);
    QJsonObject GenerateSendFileCommand(const QString &session_id, const QString &msg_id, const QString &path);

public:
    QSharedPointer<AccountConfig> account_config_;

private:
    QSharedPointer<QWebSocket> client_websocket_;
    SessionModel* session_model_ = nullptr;

    QVariantMap login_params_;
    QVariantMap profile_;
    int task_id_ = 0;
    QMap<int, CommandDelegate> command_delegate_map_;
    GroupInvitedModel* invited_group_model_ = nullptr;
    QString my_mid_;


    struct ContactInfo {
      ContactItem contact;
      int check_time = 0;
    };

    QMap<QString, ContactInfo> mid_contact_map_;
};

#endif // LINEENGINE_H
