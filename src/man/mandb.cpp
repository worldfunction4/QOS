#include "mandb.h"

#include <QDateTime>

Q_GLOBAL_STATIC(ManDB, ins)


ManDB::ManDB(QObject *parent)
    : QObject{parent}
{}

ManDB *ManDB::instance()
{
    return ins();
}

void ManDB::init()
{
    daologininfo_.connect();
    daologininfo_.createTable();
}

void ManDB::saveLoginInfo(
    const QString &name, 
    const QString &id, 
    const QString &key, 
    const QString &remark)
{
    LoginInfo info;
    info.name = (name == ""?id:name);
    info.remark = remark.trimmed();
    info.secretId = id.trimmed();
    info.secretKey = key.trimmed();
    info.timeStamp = QDateTime::currentSecsSinceEpoch();

    //查询是否存在这个id？
    if(daologininfo_.exists(info.secretId))
    {
        daologininfo_.update(info);
    }
    else
    {
        daologininfo_.insert(info);
    }
}

void ManDB::removeLoginInfo(const QString &id)
{
    if(daologininfo_.exists(id))
    {
        daologininfo_.remove(id);
    }
}