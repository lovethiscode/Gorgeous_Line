#include "lineengine.h"
#include "globalconfig.h"
#include <QtDebug>
#include "modelitem.h"
#include "util/quicklist.h"
#include "util/fileutil.h"

#define MAX_CHECK_CONTACT_TIME (5 * 60)

LineEngine::LineEngine(QObject *parent) : QObject(parent)
{
}

LineEngine::~LineEngine()
{
}

LineEngine *LineEngine::Instance()
{
  static LineEngine* s_instance = new LineEngine();
  return s_instance;
}

//只会有一个session model
SessionModel *LineEngine::getSessionModel()
{
    //qml 会销毁，这里只是保存，不能删除
    if(session_model_ == nullptr) {
        session_model_ = new SessionModel();
    }
    return session_model_;
}

void LineEngine::deleteSession(int row, const QString& id)
{
    session_model_->RemoveSession(id);
    account_config_->DeleteSession(id);
}

ContactModel *LineEngine::getContactModel()
{
   ContactModel* contact_model = new ContactModel;
   contact_model->SetContacts(account_config_->GetContacts());
   return contact_model;
}

GroupModel *LineEngine::getGroupModel()
{
  GroupModel* group_model = new GroupModel;
  group_model->SetGroups(account_config_->GetGroups());
  return group_model;
}
/*
    QVariantMap info;
    info["user"] = "abc";
    info["password"] = "56";
    info["token"] = "token3";
    info["type"] = 2;

    QVariantMap info;
    info["user"] = "abc";
    info["token"] = "token3";
    info["type"] = 2;

*/
void LineEngine::login(const QVariantMap &params)
{
    login_params_ = params;

    if(account_config_ == nullptr) {
        account_config_ =  QSharedPointer<AccountConfig>::create(login_params_["user"].toString());
    }

    QVariantMap server_info = GlobalConfig::Instance().getServerInfo();

    client_websocket_ = QSharedPointer<QWebSocket>::create();

    connect(client_websocket_.get(), &QWebSocket::connected, this,
             &LineEngine::OnConnected);
     connect(client_websocket_.get(), &QWebSocket::disconnected, this,
             &LineEngine::OnDisconnect);
     connect(client_websocket_.get(), &QWebSocket::textMessageReceived, this,
             &LineEngine::OnMessageReceived);

     connect(client_websocket_.get(), QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this,
             &LineEngine::OnError);

     client_websocket_->open(QUrl(QString("ws://%1:%2").arg(server_info["server"].toString()).arg(server_info["port"].toInt())));
}

int LineEngine::addSession(const QString& mid, int type)
{
  switch (type) {
    case EnumDefine::SessionTypePrivate: {
      AddContactSessionIfNeed(mid);
    }
      break;
    case EnumDefine::SessionTypeGroup:{
      AddGroupSessionIfNeed(mid);
    }
      break;
  }
  return 1;
}

void LineEngine::setNickname(const QString &nickname)
{
    QJsonObject command;
    command["command"] = "SetNickName";
    command["nickname"] = nickname;

    SendCommand(command);
}

void LineEngine::setSignature(const QString &signature)
{
    QJsonObject command;
    command["command"] = "SetSignature";
    command["signature"] = signature;

    SendCommand(command);
}

void LineEngine::checkUserIdIsValid(const QString &userid)
{
    QJsonObject command;
    command["command"] = "CheckUseridValid";
    command["user_id"] = userid;

    SendCommand(command);
}

void LineEngine::setUserId(const QString &userid)
{
    QJsonObject command;
    command["command"] = "SetMyUserid";
    command["user_id"] = userid;

    SendCommand(command);
}

void LineEngine::getQrcode()
{
    QJsonObject command;
    command["command"] = "GetQrCode";

    SendCommand(command);
}

void LineEngine::enableSearchByUserid(bool enable)
{
    QJsonObject command;
    command["command"] = "EnableSearchByUserId";
    command["enable"] = enable;


    SendCommand(command);
}

void LineEngine::setBirthday(const QString &year, const QString &day)
{
    QJsonObject command;
    command["command"] = "SetBirthday";
    command["year"] = year;
    command["day"] = day;

    SendCommand(command);
}

void LineEngine::acceptGroupInvited(const QString &groupid, bool accept)
{
    if(!accept) {
        //拒绝 更新数据库
        account_config_->UpdateInvitedGroup(groupid, 2);
        invited_group_model_->SetStatus(groupid, 2);
    } else {
        account_config_->UpdateInvitedGroup(groupid, 1);
        invited_group_model_->SetStatus(groupid, 1);

        QJsonObject command;
        command["command"] = "AcceptGroupInvitation";
        command["group_id"] = groupid;


        SendCommand(command);
    }
}

void LineEngine::deleteInvitedGroupsModel()
{
  delete invited_group_model_;
  invited_group_model_ = nullptr;
}

EventModel *LineEngine::getEventModel(const QString &id)
{
  return session_model_->getEventModel(id);
}

const QString &LineEngine::GetMyMid()
{
  return my_mid_;
}

const ContactItem & LineEngine::GetContact(const QString &mid)
{
  auto iter = mid_contact_map_.find(mid);
  if(iter != mid_contact_map_.end()) {
    bool need_re_query = false;
    if(iter.value().contact.display_name.isEmpty()) {
      //判读时间间隔是否大于5分钟，5分钟才会重新查询一次
      int current_time = QDateTime::currentSecsSinceEpoch();
      if (current_time > iter.value().check_time + MAX_CHECK_CONTACT_TIME) {
        need_re_query = true;
      }
    }
    if(!need_re_query) {
      return iter.value().contact;
    } else {
      mid_contact_map_.erase(iter);
    }
  }
  //从数据库查询一次
  ContactInfo contact;
  if (account_config_->GetContact(mid, contact.contact)) {
    return mid_contact_map_.insert(mid, std::move(contact)).value().contact;
  } else {
    contact.contact.mid = mid;
    contact.check_time = QDateTime::currentSecsSinceEpoch();
    return mid_contact_map_.insert(mid, contact).value().contact;
  }
}

int LineEngine::searchContact(const QString &user_id)
{
  QJsonObject command;
    command["command"] = "SearchContactByUserid";
    command["user_id"] = user_id;
    return SendCommand(command);
}


void LineEngine::InnerSendText(const QString &session_id, const QString &msg_id, const QString &text)
{
    QJsonObject command;
    command["command"] = "SendText";
    command["to"] = session_id;
    command["content"] = text;
    command["msg_id"] = msg_id;
    SendCommand(command, [=](const QString& command, const QJsonObject& json){
        session_model_->UpdateChatMessage(session_id, msg_id, json["code"].toInt() == 0 ? EnumDefine::EventItemStatusSuccess : EnumDefine::EventItemStatusFailed, AbstractEventRoleName::Status);
    });
}

void LineEngine::sendText(const QString& session_id, const QString& text)
{
  QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());
  InnerSendText(session_id, msg_id, text);

  EventItemData event;
  event.content = text;
  event.isMe = true;
  event.type = EnumDefine::EventTypeText;
  event.sender_id = my_mid_;
  event.sender_name = profile_["displayName"].toString();
  event.status = EnumDefine::EventItemStatusSending;
  event.id = msg_id;
  event.timestamp = QDateTime::currentSecsSinceEpoch();

  session_model_->AddChatMessage(session_id, event);
}

void LineEngine::reSendText(const QString &session_id, const QString &msg_id, const QString &text)
{
    InnerSendText(session_id, msg_id, text);
    session_model_->UpdateChatMessage(session_id, msg_id, EnumDefine::EventItemStatusSending, AbstractEventRoleName::Status);
}

QJsonObject LineEngine::GenerateSendImageCommand(const QString &session_id, const QString &msg_id, const QString &path)
{
    QJsonObject command;
    command["command"] = "GetSendImageParam";
    command["to"] = session_id;
    command["path"] = path;
    command["msg_id"] = msg_id;
    return command;
}

void LineEngine::sendImage(const QString &session_id, const QString &path)
{
    QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());   
    QJsonObject command = GenerateSendImageCommand(session_id, msg_id, path);
    HandleSendMedia(command, session_id, msg_id, path, EnumDefine::EventTypeImage, false);
}

void LineEngine::sendImageData(const QString &session_id, const QVariant &data)
{
    QString path = FileUtil::SaveImageToFile(data);

    QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());
    QJsonObject command = GenerateSendImageCommand(session_id, msg_id, path);
    HandleSendMedia(command, session_id, msg_id, path, EnumDefine::EventTypeImage, false);
}

void LineEngine::reSendImage(const QString &session_id, const QString &msg_id, const QString &path)
{
    QString normal_path = FileUtil::ToStandardPath(path);
    QJsonObject command = GenerateSendImageCommand(session_id, msg_id, normal_path);
    HandleSendMedia(command, session_id, msg_id, normal_path, EnumDefine::EventTypeImage, true);
}

QJsonObject LineEngine::GenerateSendVideoCommand(const QString &session_id, const QString &msg_id, const QString &path)
{
    QJsonObject command;
    command["command"] = "GetSendVideoParam";
    command["to"] = session_id;
    command["path"] = path;
    command["duration"] = FileUtil::GetMediaLength(path);
    command["msg_id"] = msg_id;
    return command;
}

void LineEngine::sendVideo(const QString &session_id, const QString &path)
{
    QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());
    QJsonObject command = GenerateSendVideoCommand(session_id, msg_id, path);

    HandleSendMedia(command, session_id, msg_id, path, EnumDefine::EventTypeVideo, false);
}

void LineEngine::reSendVideo(const QString &session_id, const QString &msg_id, const QString &path)
{
    QString normal_path = FileUtil::ToStandardPath(path);
    QJsonObject command = GenerateSendVideoCommand(session_id, msg_id, normal_path);
    HandleSendMedia(command, session_id, msg_id, normal_path, EnumDefine::EventTypeVideo, true);
}


QJsonObject LineEngine::GenerateSendAudioCommand(const QString &session_id, const QString &msg_id, const QString &path)
{
    QJsonObject command;
    command["command"] = "GetSendAudioParam";
    command["to"] = session_id;
    command["path"] = path;
    command["duration"] = FileUtil::GetMediaLength(path);
    command["msg_id"] = msg_id;
    return command;
}

void LineEngine::sendAudio(const QString &session_id, const QString &path)
{
    QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());
    QJsonObject command = GenerateSendAudioCommand(session_id, msg_id, path);
    HandleSendMedia(command, session_id, msg_id, path, EnumDefine::EventTypeAudio, false);
}

void LineEngine::reSendAudio(const QString &session_id, const QString& msg_id, const QString &path)
{
    QString normal_path = FileUtil::ToStandardPath(path);
    QJsonObject command = GenerateSendAudioCommand(session_id, msg_id, normal_path);
    HandleSendMedia(command, session_id, msg_id, normal_path, EnumDefine::EventTypeAudio, true);
}


QJsonObject LineEngine::GenerateSendFileCommand(const QString &session_id, const QString &msg_id, const QString &path)
{
    QJsonObject command;
    command["command"] = "GetSendFileParam";
    command["to"] = session_id;
    command["path"] = path;
    command["msg_id"] = msg_id;
    return command;
}


void LineEngine::sendFile(const QString &session_id, const QString &path)
{
  QString msg_id = QString::number(QDateTime::currentMSecsSinceEpoch());
  QJsonObject command = GenerateSendFileCommand(session_id, msg_id, path);
  HandleSendMedia(command, session_id, msg_id, path, EnumDefine::EventTypeFile, false);
}

void LineEngine::reSendFile(const QString &session_id, const QString &msg_id, const QString &path)
{
    QString normal_path = FileUtil::ToStandardPath(path);
    QJsonObject command = GenerateSendFileCommand(session_id, msg_id, normal_path);
    HandleSendMedia(command, session_id, msg_id, normal_path, EnumDefine::EventTypeFile, true);
}

void LineEngine::DownloadMedia(const QString session_id, const QString &msgid, const QString &path)
{
    QJsonObject command;
    command["command"] = "GetDownloadFileParam";
    command["msg_id"] = msgid;
    command["path"] = path;

    SendCommand(command, [=](const QString& command, const QJsonObject& json){
        QJsonObject content = ToJsonObject(json["content"].toString());
        QJsonObject json_heads = content["heads"].toObject();
        QMap<QString, QString> heads;
        for(auto begin = json_heads.begin(); begin != json_heads.end(); begin++){
          heads[begin.key()] = begin.value().toString();
        }
        QString url = content["url"].toString();
        QString path = content["path"].toString();
        HttpUploadManager::Instance()->DownloadFile(url, path, heads, [=](int code) {
            session_model_->UpdateChatMessage(session_id, msgid, path, AbstractEventRoleName::Source);
        });
    });
}

void LineEngine::downloadMedia(const QString session_id, const QString &msgid, const QString& title)
{
    QDir image_dir(account_config_->GetImageCacheDir());
    QString path = image_dir.absoluteFilePath(msgid + "_" + title);
    DownloadMedia(session_id, msgid, path);
}

GroupInvitedModel* LineEngine::getInvitedGroupsModel()
{
  auto invited_groups = account_config_->GetInvitedGroups();
  for(auto& group : invited_groups) {
    if(group.status == -1) {
        //需要获取群状态
        GetInviteGroupInfo(group);
    }
  }
  invited_group_model_ = new GroupInvitedModel;
  invited_group_model_->SetData(std::move(invited_groups));
  return invited_group_model_;
}

void LineEngine::deleteGroupInvited(const QString &group_id)
{
    account_config_->DeleteInvitedGroup(group_id);
    if(nullptr != invited_group_model_) {
        invited_group_model_->RemoveItem(group_id);
    }
}

void LineEngine::OnConnected()
{
    qDebug() << "connect server";
    //开始登陆
    QJsonObject command;
    int type = login_params_["type"].toInt();
    if(type == 0) {
       command["command"] = "Login";
       command["email"] = login_params_["user"].toString();
       command["password"] = login_params_["password"].toString();
       //第一次登陆的时候 为空字符串，需要在手机上输入Pin code， 登陆成功之后会收到回调，客户自己保存，下次直接使用
       command["cert"] = login_params_["token"].toString();
    } else if(type == 1) {
        command["command"] = "LoginWithToken";
        command["phone"] = login_params_["user"].toString();
        command["auth_token"] = login_params_["token"].toString();
    }

    QVariantMap proxy = GlobalConfig::Instance().getProxyInfo();
    if(proxy["enableProxy"].toBool()) {
        //设置代理
        QString proxyServer = proxy["proxyServer"].toString();
        command["proxy_server"] = proxyServer;

        int proxyPort = proxy["proxyPort"].toInt();
        command["proxy_port"] = proxyPort;

        QString proxy_username = proxy["proxyUsername"].toString();
        command["proxy_username"] =proxy_username;

        QString pass_word = proxy["proxyPassword"].toString();
        command["proxy_password"] = pass_word;
        //ProxyType  socks5     http  http支持密码， socks5不支持密码
       command["proxy_type"] = proxy["proxyType"].toInt() == 0 ? "socks5" : "http";

       QNetworkProxy upload_proxy;
       upload_proxy.setType(proxy["proxyType"].toInt() == 0 ? QNetworkProxy::Socks5Proxy : QNetworkProxy::HttpProxy);
       upload_proxy.setHostName(proxyServer);
       upload_proxy.setPort(proxyPort);
       upload_proxy.setUser(proxy_username);
       upload_proxy.setPassword(pass_word);


       HttpUploadManager::Instance()->SetProxy(upload_proxy);
    } else {
      QNetworkProxy upload_proxy;
      HttpUploadManager::Instance()->SetProxy(upload_proxy);
    }
    SendCommand(command);
}

void LineEngine::OnDisconnect()
{
    qDebug() << "disconnect server";
    QJsonObject result;
    emit commandResponse("Disconnect", result);
}

void LineEngine::OnMessageReceived(const QString &message)
{
    qDebug() << "Received:" << message;
    //先解析json格式
    QJsonParseError jsonError;
    QJsonDocument doucment =
        QJsonDocument::fromJson(message.toUtf8(), &jsonError);
    if (!doucment.isObject()) {
      return;
    }
    QJsonObject root = doucment.object();
    QString command = root["command"].toString();
    if(root.contains("task_id")) {
        int task_id = root["task_id"].toInt();
        auto iter = command_delegate_map_.find(task_id);
        if(iter != command_delegate_map_.end()) {
            iter.value()(command, root);
            command_delegate_map_.erase(iter);
            return;
        }
    }

    if (InterceptResponse(command, root)){
        return;
    }
    emit commandResponse(command, root);
}

void LineEngine::OnError(const QAbstractSocket::SocketError &error)
{
    //qDebug() << "OnError:" << error;
}

int LineEngine::SendCommand(QJsonObject &command, CommandDelegate delegate)
{
    //加上task_id 参数
    int task_id = ++task_id_;
    command["task_id"] = task_id;

    QJsonDocument document;
    document.setObject(command);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    client_websocket_->sendTextMessage(QString(byteArray));
    if(nullptr != delegate) {
        command_delegate_map_[task_id] = std::move(delegate);
    }
    return task_id;
}

bool LineEngine::InterceptResponse(const QString &command, const QJsonObject &json)
{
    //如果是登录成功，获取一下配置和拉取好友
    if(command == "OnLogin") {
         HandleLogin(json);
    } else if (command == "GetProfile") {
        HandleProfile(json);
        return true;
    } else if (command == "GetAllContactIds") {
        HandleAllContact(json);
        return true;
    }  else if (command == "GetJoinedGroupIds") {
        HandleJoinedGroups(json);
        return true;
    } else if (command == "GetContacts") {
        HandleGetContacts(json);
        return true;
    } else if (command == "SearchGroupById") {
        HandleGetGroups(json);
        return true;
    } else if (command == "GetQrCode") {
        HandleGetQrCode(json);
        return true;
    } else if (command == "SetSignature") {
        HandleSetSignature(json);
    } else if (command == "SetNickName") {
        HandleSetNickName(json);
    } else if (command == "OnNotify") {
        return HandleNotify(json);
    }

    return false;
}

void LineEngine::HandleLogin(const QJsonObject& root)
{
    if(0 != root["code"].toInt())
    {
        return;
    }
    //更新账号信息
    {
        AccountInfo account;
        account.account = login_params_["user"].toString();
        account.password = login_params_["password"].toString();
        account.token = login_params_["token"].toString();
        account.type = (AccountType)login_params_["type"].toInt();

        GlobalConfig::Instance().UpdateAccount(account);
    }

    InitSessions();
    //获取配置
    {
         QJsonObject command;
         command["command"] = "GetProfile";
         SendCommand(command);
    }
    //获取联系人
    {
         QJsonObject command;
         command["command"] = "GetAllContactIds";
         SendCommand(command);
    }
    //获取加入的群
    {
        QJsonObject command;
        command["command"] = "GetJoinedGroupIds";
        SendCommand(command);
    }
}

void LineEngine::HandleProfile(const QJsonObject &root)
{
  QJsonObject profile = ToJsonObject(root["content"].toString());
  profile_ = profile.toVariantMap();
  //添加一些自定义字段
  profile_.insert("qrcode", account_config_->GetSetting(SETTING_QRCODE));
  my_mid_ = profile_["mid"].toString();
  emit notifyProfileChanged();
}

void LineEngine::HandleAllContact(const QJsonObject &root)
{
   QJsonArray contact_ids = ToJsonArray(root["content"].toString());
   //获取好有详细信息
   GetContacts(contact_ids);
}

void LineEngine::HandleJoinedGroups(const QJsonObject &root)
{
   SearchGroups(ToJsonArray(root["content"].toString()));
}



void LineEngine::SearchGroups(const QJsonArray &ids, CommandDelegate delegate)
{
    QJsonObject command;
    command["command"] = "SearchGroupById";
    command["mids"] = ids;
    SendCommand(command, delegate);
}

void LineEngine::GetContacts(const QJsonArray &ids, CommandDelegate delegate)
{
  QJsonObject command;
  command["command"] = "GetContacts";
  command["mids"] = ids;

  SendCommand(command, delegate);
}

void LineEngine::HandleSendMedia(QJsonObject &command, const QString& session_id, const QString& msg_id, const QString& path, EnumDefine::EventType type, bool retry)
{
  SendCommand(command, [=] (const QString& command, const QJsonObject& json){
      UploadMedia(json, session_id, msg_id);
  });
  if(retry) {
      session_model_->UpdateChatMessage(session_id, msg_id, EnumDefine::EventItemStatusSending, AbstractEventRoleName::Status);
  } else {
      EventItemData event;
      event.isMe = true;
      event.type = type;
      event.sender_id = my_mid_;
      event.sender_name = profile_["displayName"].toString();
      event.status = EnumDefine::EventItemStatusSending;
      event.id = msg_id;
      event.timestamp = QDateTime::currentSecsSinceEpoch();
      event.source = path;
      QFileInfo info(path);
      event.title = info.baseName();
      event.size = info.size();

      session_model_->AddChatMessage(session_id, event);
  }
}

void LineEngine::UploadMedia(const QJsonObject& json, const QString& session_id, const QString& msg_id)
{
  if(0 == json["code"].toInt()) {
     UploadObject(json, [=](int code){
        if(code == 0) {
            session_model_->UpdateChatMessage(session_id, msg_id, EnumDefine::EventItemStatusSuccess, AbstractEventRoleName::Status);
        } else {
            session_model_->UpdateChatMessage(session_id, msg_id, EnumDefine::EventItemStatusFailed, AbstractEventRoleName::Status);
        }
    });
  } else {
    session_model_->UpdateChatMessage(session_id, msg_id, EnumDefine::EventItemStatusFailed, AbstractEventRoleName::Status);
  }
}

void LineEngine::AutoDownloadMedia(const QString& session_id, const EventItemData &event)
{
    switch (event.type) {
        case EnumDefine::EventTypeImage:{
            QDir image_dir(account_config_->GetImageCacheDir());
            QString path = image_dir.absoluteFilePath(event.id + ".jpg");
            DownloadMedia(session_id, event.id, path);
            session_model_->UpdateChatMessage(session_id, event.id, EnumDefine::EventItemStatusDownloading, AbstractEventRoleName::Status);
        }
        break;
        case EnumDefine::EventTypeVideo:{
            QDir image_dir(account_config_->GetVideoCacheDir());
            QString path = image_dir.absoluteFilePath(event.id + ".mp4");
            DownloadMedia(session_id, event.id, path);
            session_model_->UpdateChatMessage(session_id, event.id, EnumDefine::EventItemStatusDownloading, AbstractEventRoleName::Status);
        }
        break;
        case EnumDefine::EventTypeAudio:{
            QDir image_dir(account_config_->GetAudioCacheDir());
            QString path = image_dir.absoluteFilePath(event.id + ".mp3");
            DownloadMedia(session_id, event.id, path);
            session_model_->UpdateChatMessage(session_id, event.id, EnumDefine::EventItemStatusDownloading, AbstractEventRoleName::Status);
        }
        break;
        case EnumDefine::EventTypeFile: {
            QDir image_dir(account_config_->GetFileCacheDir());
            QString path = image_dir.absoluteFilePath(event.id + event.title);
            DownloadMedia(session_id, event.id, path);
            session_model_->UpdateChatMessage(session_id, event.id, EnumDefine::EventItemStatusDownloading, AbstractEventRoleName::Status);
          }
            break;
    }
}

void LineEngine::HandleGetContacts(const QJsonObject &root)
{
    QJsonArray json_contacts = ToJsonArray(root["content"].toString());
    auto contacts = ContactItem::FromJsonArray(json_contacts);
    account_config_->UpdateContacts(contacts);
    //判断是否要更新session
    UpdateSessionIfNeed(contacts);
}

void LineEngine::HandleGetGroups(const QJsonObject &root)
{
    QJsonArray json_groups = ToJsonArray(root["content"].toString());
    auto groups = GroupItem::FromJsonArray(json_groups);
    account_config_->UpdateGroups(groups);
    //判断是否需要更新session
    UpdateSessionIfNeed(groups);
}

void LineEngine::HandleGetQrCode(const QJsonObject &root)
{
    QJsonObject qrcode_json = ToJsonObject(root["content"].toString());
    QString qrcode = qrcode_json["url"].toString();
    account_config_->SetSetting(SETTING_QRCODE, qrcode);
    profile_["qrcode"] = qrcode;

    emit notifyProfileChanged();
}

void LineEngine::HandleSetSignature(const QJsonObject &root)
{
    QJsonObject json = ToJsonObject(root["content"].toString());
    profile_["statusMessage"] = json["signature"].toString();
    emit notifyProfileChanged();
}

void LineEngine::HandleSetNickName(const QJsonObject &root)
{
    QJsonObject json = ToJsonObject(root["content"].toString());
    profile_["displayName"] = json["nickname"].toString();
    emit notifyProfileChanged();
}

bool LineEngine::HandleNotify(const QJsonObject &root)
{
    const QString sub_command = root["subcommand"].toString();
    if(sub_command == "RECEIVE_MESSAGE") {
        HandleReceiveMessage(ToJsonObject(root["content"].toString()));
    } else if(sub_command == "NOTIFIED_INVITE_INTO_CHAT") {
        HandleInviteIntoChat(ToJsonObject(root["content"].toString()));
    } else if(sub_command == "NOTIFIED_CANCEL_CHAT_INVITATION") {
        HandleCancelInviteIntoChat(ToJsonObject(root["content"].toString()));
    } else if(sub_command == "ACCEPT_CHAT_INVITATION") {
        HandleAcceptChatInvitation(ToJsonObject(root["content"].toString()));
    } else if(sub_command == "NOTIFIED_LEAVE_CHAT") {
        HandleLeaveChat(ToJsonObject(root["content"].toString()));
    } else if(sub_command == "NOTIFIED_UPDATE_PROFILE") {
        HandleUpdateProfile(ToJsonObject(root["content"].toString()));
    }

    return true;
}

void LineEngine::HandleReceiveMessage(const QJsonObject &message)
{
    QString content_type = message["contentType"].toString();
    if((content_type == "NONE") || (content_type == "CONTACT")
            || (content_type == "FILE") || (content_type == "IMAGE")
            || (content_type == "VIDEO") || (content_type == "AUDIO")
            ||  (content_type == "CONTACT")) {
      HandleReceiveChatMessage(message);
    } else if(content_type == "CHATEVENT") {
        HandleChatEvent(message);
    }
}

void LineEngine::HandleChatEvent(const QJsonObject &message)
{
    QJsonObject contentMetadata = message["contentMetadata"].toObject();
    QString to = message["to"].toString();
    QString event_type = contentMetadata["event_type"].toString();
    if(event_type == "update_group_name") {
        account_config_->UpdateGroupName(to, contentMetadata["name"].toString());
    }
}

void LineEngine::HandleReceiveChatMessage(const QJsonObject &message)
{
   //判断session 是否存在，如果不存在添加session
  QString from = message["from"].toString();
  QString to = message["to"].toString();
  MidType mid_type = GetMidType(to);
  switch (mid_type) {
    case MidTypeGroup:{
      AddGroupSessionIfNeed(to);
    }
      break;
    case MidTypeUser: {
      AddContactSessionIfNeed(from);
    }
      break;
    default:{
      qDebug() << "HandleChatMessage unsupport type:" << mid_type;
      return;
    }
  }
  //将聊天记录插入到 EventModel
  EventItemData event = EventItemData::FromJsonObject(message);
  QString session_id = mid_type == MidTypeGroup? to : from;
  session_model_->AddChatMessage(session_id, event);
  AutoDownloadMedia(session_id, event);
}

static SessionItem ContactToSession(const ContactItem& contact) {
  SessionItem session;
  session.id = contact.mid;
  session.avatar_url = contact.picture_path;
  session.display_name = contact.display_name;
  session.type = EnumDefine::SessionTypePrivate;
  session.status_msg = contact.status_message;
  return session;
}

static SessionItem GroupToSession(const GroupItem& contact) {
  SessionItem session;
  session.id = contact.mid;
  session.avatar_url = contact.picture_path;
  session.display_name = contact.display_name;
  session.type = EnumDefine::SessionTypeGroup;
  return session;
}

void LineEngine::AddContactSessionIfNeed(const QString &mid)
{
  if (!session_model_->HasSession(mid)){
    //检查是否存在联系人
    ContactItem item;
    if (!account_config_->GetContact(mid, item)) {
      //保存到数据库
      item.mid = mid;
      account_config_->UpdateContacts(QList<ContactItem>() << item);
      //搜索一下联系人
      GetContacts(QJsonArray() << mid);
    }
    //插入一个session
    SessionItem session = ContactToSession(item);
    session.timestamp = QDateTime::currentSecsSinceEpoch();
    session_model_->AddSession(0, session);
    account_config_->UpdateSession(session);
  }
}

void LineEngine::AddGroupSessionIfNeed(const QString &mid)
{
  if (!session_model_->HasSession(mid)){
    //检查是否存在群组
    GroupItem item;
    if (!account_config_->GetGroup(mid, item)) {
      //保存到数据库
      item.mid = mid;
      account_config_->UpdateGroups(QList<GroupItem>() << item);
      //搜索一下群组
      SearchGroups(QJsonArray() << mid);
    }
    //插入一个session
    SessionItem session = GroupToSession(item);
    session.timestamp = QDateTime::currentSecsSinceEpoch();
    session_model_->AddSession(0, session);
    account_config_->UpdateSession(session);
  }
}

void LineEngine::UpdateSessionIfNeed(const QList<ContactItem> &contacts)
{
  for(auto& contact : contacts) {
    if (!session_model_->HasSession(contact.mid)){
      continue;
    }
    SessionItem session = ContactToSession(contact);
    session_model_->UpdateSession(session);
    account_config_->UpdateSession(session);
  }
}

void LineEngine::UpdateSessionIfNeed(const QList<GroupItem> &groups)
{
  for(auto& group : groups) {
    if (!session_model_->HasSession(group.mid)){
      continue;
    }
    SessionItem session = GroupToSession(group);
    session_model_->UpdateSession(session);
    account_config_->UpdateSession(session);
  }
}

void LineEngine::HandleInviteIntoChat(const QJsonObject &message)
{
  InvitedIntoGroupItem group;
  group.invitee = message["mid"].toString();
  group.group_item.mid = message["group_id"].toString();
  group.timestamp = message["created_time"].toInt();
  group.status = -1;
  account_config_->UpdateInvitedGroups(group);
  if(nullptr != invited_group_model_) {
    invited_group_model_->UpdateInvitedGroups(group);
  }
  GetInviteGroupInfo(group);
}

void LineEngine::HandleCancelInviteIntoChat(const QJsonObject &message)
{
    QString group_id = message["group_id"].toString();
    deleteGroupInvited(group_id);
}

void LineEngine::HandleAcceptChatInvitation(const QJsonObject &message)
{
    GroupItem item;
    item.mid = message["group_id"].toString();
    //先保存到数据库
    account_config_->UpdateGroups(QList<GroupItem>() << item);
    SearchGroups(QJsonArray() << item.mid);
}

void LineEngine::HandleLeaveChat(const QJsonObject &message)
{
    account_config_->DeleteGroup(message["group_id"].toString());
}

void LineEngine::HandleUpdateProfile(const QJsonObject &message)
{
    int type = message["type"].toString().toInt();
    QString mid = message["mid"].toString();
    switch (type) {
        case 2:{
            //修改名字
          QJsonObject params = ToJsonObject(message["params"].toString());
          account_config_->UpdateContactName(mid, params["DISPLAY_NAME"].toString());
        }
        break;
        case 8: {
            //修改头像 重新获取下联系人
          GetContacts(QJsonArray() << mid);
        }
        break;
    }
}


QJsonArray LineEngine::ToJsonArray(const QString& json) {
    QJsonParseError jsonError;
    QJsonDocument doucment =
        QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    if (!doucment.isArray()) {
      return QJsonArray();
    }
    return doucment.array();
}

QJsonObject LineEngine::ToJsonObject(const QString &json)
{
    QJsonParseError jsonError;
    QJsonDocument doucment =
        QJsonDocument::fromJson(json.toUtf8(), &jsonError);
    if (!doucment.isObject()) {
      return QJsonObject();
    }
    return doucment.object();
}

void LineEngine::InitSessions()
{
    QList<SessionItem> sessions = account_config_->GetSessions();
    session_model_->SetSessions(std::move(sessions));
}

void LineEngine::UploadObject(const QJsonObject &data, UploadDelegate delegate)
{
  QJsonObject content = ToJsonObject(data["content"].toString());
  QJsonObject json_heads = content["heads"].toObject();
  QMap<QString, QString> heads;
  for(auto begin = json_heads.begin(); begin != json_heads.end(); begin++){
    heads[begin.key()] = begin.value().toString();
  }
  QString url = content["url"].toString();
 // QString url = "http://220.181.38.251";
  QString params = content["params"].toString();
  QString path = content["path"].toString();
 // QString path = "F:\\td\\tdjava\\11.png";
  HttpUploadManager::Instance()->UploadFile(url, path, params, heads, delegate);
}

void LineEngine::GetInviteGroupInfo(const InvitedIntoGroupItem &group)
{
    //发送请求获取群信息
    SearchGroups(QJsonArray() << group.group_item.mid,  [=](const QString& command, const QJsonObject& json) {
          QList<GroupItem> search_groups =  GroupItem::FromJsonArray(ToJsonArray(json["content"].toString()));
          if(search_groups.length() != 1) {
              qDebug() << "search group error:" << group.group_item.mid;
              return ;
          }
          InvitedIntoGroupItem search_invited_group = group;
          search_invited_group.group_item = search_groups.at(0);
          search_invited_group.status = 0;// 已经获取到了信息
          account_config_->UpdateInvitedGroups(search_invited_group);
          if(nullptr != invited_group_model_) {
            invited_group_model_->UpdateInvitedGroups(search_invited_group);
          }
     });
}
