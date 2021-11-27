#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
#include <QString>
#include <QSqlDatabase>
#include <QObject>
#include <QList>
#include <QVariantMap>
#include "modelitem.h"

class GlobalConfig :public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantMap serverInfo READ getServerInfo WRITE writeServerInfo)
    Q_PROPERTY(QVariantMap proxyInfo READ getProxyInfo WRITE writeProxyInfo)

public:
    GlobalConfig(QObject *parent = nullptr);
    static GlobalConfig& Instance();

    //  服务器信息
    QVariantMap getServerInfo();
    void writeServerInfo(const QVariantMap& server_info);
    //代理信息
    QVariantMap getProxyInfo();
    void writeProxyInfo(const QVariantMap& server_info);


    //qml  使用
    Q_INVOKABLE QVariantMap getAccounts();

    void UpdateAccount(const AccountInfo& account);
    QList<AccountInfo> GetAccounts();

private:
    QVariantMap GetVariantMap(const QString& key);
    void WriteVariantMap(const QString& key, const QVariantMap& accounts);

    void CreateTable();
    QSqlDatabase db_;
};


#endif // GLOBALCONFIG_H
