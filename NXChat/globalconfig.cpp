#include "globalconfig.h"
#include <QApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDataStream>
#include <QBuffer>

const char* SERVER_INFO = "server";
const char* PROXY_INFO = "proxy";

GlobalConfig &GlobalConfig::Instance()
{
    static GlobalConfig* s_instance = new GlobalConfig;
    return *s_instance;
}

void GlobalConfig::CreateTable()
{
    {
      //创建设置表
      QSqlQuery query(db_);
      query.exec(
          "CREATE TABLE IF NOT EXISTS setting (keys text primary  key, value BLOB);");
    }

    {
        //账号表
        QSqlQuery query(db_);
        query.exec("CREATE TABLE IF NOT EXISTS accounts (account text primary  key, password text, token text, type int);");
    }
}

QVariantMap GlobalConfig::getServerInfo()
{
    QVariantMap result;
    QSqlQuery query(db_);
    query.prepare("select * from setting where keys = ?;");
    query.addBindValue(SERVER_INFO);
    query.exec();
    if (query.next()) {
        QDataStream stream(query.value("value").toByteArray());
        stream >> result;
    }
    return result;
}

void GlobalConfig::writeServerInfo(const QVariantMap &server_info)
{
     QBuffer data;
     data.open(QIODevice::WriteOnly);
     QDataStream stream(&data);
     stream << server_info;
     QSqlQuery query(db_);
     query.prepare("INSERT OR REPLACE INTO setting (keys, value) values(?, ?)");
     query.addBindValue(SERVER_INFO);
     query.addBindValue(data.data());
     query.exec();
}

QVariantMap GlobalConfig::getProxyInfo()
{
    return GetVariantMap(PROXY_INFO);
}

void GlobalConfig::writeProxyInfo(const QVariantMap &server_info)
{
    WriteVariantMap(PROXY_INFO, server_info);
}
/*
        QVariantMap info;
        info["password"] = "56";
        info["token"] = "token3";
        info["type"] = 2;
        account["3456"] = std::move(info);

*/
QVariantMap GlobalConfig::getAccounts()
{
    QVariantMap map_value;
    QList<AccountInfo> accounts = GetAccounts();
    for(auto& account : accounts) {
        QVariantMap accoun_map;
        accoun_map["password"] = account.password;
        accoun_map["token"] = account.token;
        accoun_map["type"] = account.type;
        map_value[account.account] = std::move(accoun_map);
    }

    return map_value;
}

void GlobalConfig::UpdateAccount(const AccountInfo &account)
{
    QSqlQuery query(db_);
    query.prepare("INSERT OR REPLACE INTO accounts (account, password,token,type) values(?, ?, ?,?)");
    query.addBindValue(account.account);
    query.addBindValue(account.password);
    query.addBindValue(account.token);
    query.addBindValue(account.type);
    query.exec();
}

QList<AccountInfo> GlobalConfig::GetAccounts()
{
    QList<AccountInfo> result;
    QSqlQuery query(db_);
    query.prepare("select * from accounts;");
    query.exec();
    while (query.next()) {
        AccountInfo item;
        item.account = query.value("account").toString();
        item.password = query.value("password").toString();
        item.token = query.value("token").toString();
        item.type = (AccountType)query.value("type").toInt();
        result.append(std::move(item));
    }

    return result;
}

QVariantMap GlobalConfig::GetVariantMap(const QString &key)
{
    QVariantMap map_value;
    QSqlQuery query(db_);
    query.prepare("select * from setting where keys = ?;");
    query.addBindValue(key);
    query.exec();
    if (query.next()) {
        QDataStream stream(query.value("value").toByteArray());
        stream >> map_value;
    }
    return map_value;
}

void GlobalConfig::WriteVariantMap(const QString &key, const QVariantMap &map)
{
    QBuffer data;
    data.open(QIODevice::WriteOnly);
     QDataStream stream(&data);
     stream << map;
     QSqlQuery query(db_);
     query.prepare("INSERT OR REPLACE INTO setting (keys, value) values(?, ?)");
     query.addBindValue(key);
     query.addBindValue(data.data());
     query.exec();
}

GlobalConfig::GlobalConfig(QObject *parent): QObject(parent)
{
    QString exePath = QCoreApplication::applicationFilePath();
    QDir dir(exePath);
    dir.cdUp();
    dir.mkpath("config");
    QDir config_dir =QDir(dir.absoluteFilePath("config"));
    QString global_db = config_dir.absoluteFilePath("global");
    bool db_exist = QFile::exists(global_db);

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(global_db);
    if (!db_.open()) {
      QMessageBox::warning(0, QString::fromStdWString(L"Database error"),
                           db_.lastError().text());
      return;
    }
    if(!db_exist) {
        CreateTable();
    }
}
