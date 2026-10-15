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
    //预防一下输入空格的情况
    QString tri_name = name.trimmed();
    QString tri_id = id.trimmed();
    info.name = (tri_name.isEmpty() ? tri_id : tri_name);
    info.remark = remark.trimmed();
    info.secretId = tri_id;
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