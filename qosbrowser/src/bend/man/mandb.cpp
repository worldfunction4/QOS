#include "mandb.h"
#include "bend/dao/dbs/daologininfosqlite.h"

#include <QDateTime>

ManDB::ManDB(QObject *parent)
    : QObject{parent}
{
    daologininfo_ = new DaoLoginInfoSqlite;
}

void ManDB::init()
{
    daologininfo_->connect();
    daologininfo_->createTable();
    //需要在dao层定义一个select方法进行查询
    logininfo_list_ = daologininfo_->select();
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
    if(daologininfo_->exists(info.secretId))
    {
        daologininfo_->update(info);
        logininfo_list_[logininfoIndex(info.secretId)] = info;
    }
    else
    {
        daologininfo_->insert(info);
        //缓存也要插入更新
        logininfo_list_.append(info);
    }
}

void ManDB::removeLoginInfo(const QString &id)
{
    if(daologininfo_->exists(id))
    {
        daologininfo_->remove(id);
        //缓存也要删除
        logininfo_list_.removeAt(logininfoIndex(id));
    }
}

int ManDB::logininfoIndex(const QString &secretId)
{
    for(int i=0; i < logininfo_list_.size();++i)
    {
        if(logininfo_list_[i].secretId == secretId)
        {
            return i;
        }
    }
    throw QString("获取登录信息索引失败:%1").arg(secretId);
}

QStringList ManDB::loginInfoNames() const
{
    QStringList names;
    for(const LoginInfo& info : logininfo_list_)
    {
        names.append(info.name);
    }
    return names;
}

bool ManDB::loginInfoByName(const QString &name, LoginInfo *info) const
{
    //进行遍历
    for(const LoginInfo& item : logininfo_list_)
    {
        if(item.name == name)
        {
            if(info)
            {
                *info = item;
            }
            return true;
        }
    }
    return false;
}