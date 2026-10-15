#include "daologininfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include "src/config/config.h"

DaoLoginInfo::DaoLoginInfo(): table_(CONFIG::TABLE::LOGIN_INFO_NAME)
{}

bool DaoLoginInfo::exists(const QString &secret_id)
{
    //执行sql语句用
    QSqlQuery query;
    QString sql = QString("select count(*) from %1 "
                          "where secret_id = :secret_id").arg(table_);
    //进行sql预处理
    query.prepare(sql);
    query.bindValue(":secret_id", secret_id);

    if(!query.exec())
    {
        throw QString("查询登录信息失败, secret_id:%1, 错误:%2")
        .arg(secret_id)
        .arg(query.lastError().text());
    }
    if(query.next())
    {
        return query.value(0).toInt() > 0;
    }
    return false;
}

void DaoLoginInfo::insert(const LoginInfo &info)
{
    QSqlQuery query;
    QString sql = QString(
                      "INSERT INTO %1 "
                      "(name, secret_id, secret_key, remark, timestamp) "
                      "VALUES "
                      "(:name, :secret_id, :secret_key, :remark, :timestamp)"
                      ).arg(table_);
    query.prepare(sql);
    query.bindValue(":name", info.name);
    query.bindValue(":secret_id", info.secretId);
    query.bindValue(":secret_key", info.secretKey);
    query.bindValue(":remark", info.remark);
    query.bindValue(":timestamp", info.timeStamp);
    if (!query.exec())
    {
        throw QString("插入登录信息失败, secret_id:%1, 错误:%2")
            .arg(info.secretId)
            .arg(query.lastError().text());
    }
}

void DaoLoginInfo::update(const LoginInfo &info)
{
    QSqlQuery query;
    QString sql = QString(
                      "UPDATE %1 SET "
                      "name = :name, "
                      "secret_key = :secret_key, "
                      "remark = :remark, "
                      "timestamp = :timestamp "
                      "WHERE secret_id = :secret_id"
                      ).arg(table_);
    query.prepare(sql);
    query.bindValue(":name", info.name);
    query.bindValue(":secret_id", info.secretId);
    query.bindValue(":secret_key", info.secretKey);
    query.bindValue(":remark", info.remark);
    query.bindValue(":timestamp", info.timeStamp);
    if (!query.exec())
    {
        throw QString("更新登录信息失败, secret_id:%1, 错误:%2")
            .arg(info.secretId)
            .arg(query.lastError().text());
    }
}

void DaoLoginInfo::remove(const QString &secretId)
{
    QSqlQuery query;
    QString sql = QString(
                      "DELETE FROM %1 "
                      "WHERE secret_id = :secret_id"
                      ).arg(table_);
    query.prepare(sql);
    query.bindValue(":secret_id", secretId);
    if (!query.exec())
    {
        throw QString("删除登录信息失败, secret_id:%1, 错误:%2")
            .arg(secretId)
            .arg(query.lastError().text());
    }
}

void DaoLoginInfo::connect()
{
    //取消硬编码
    db_.connect(CONFIG::SQLITE::NAME);
}

void DaoLoginInfo::createTable()
{
    //创建表
    QString sql = FileHelper::readAllText(CONFIG::SQL::LOGIN_INFO_PATH);
    db_.exec(sql);
}