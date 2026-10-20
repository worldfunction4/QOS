#ifndef DAOLOGININFO_H
#define DAOLOGININFO_H

#include <QList>
#include "middle/models/dbmodels.h"

// 登录信息的数据库接口,只规定要做什么,不规定怎么做
class DaoLoginInfo
{
public:
    virtual ~DaoLoginInfo() = default;

    virtual bool exists(const QString& secret_id) = 0;
    virtual void insert(const LoginInfo& info) = 0;
    virtual void update(const LoginInfo& info) = 0;
    virtual void remove(const QString& secretId) = 0;
    virtual QList<LoginInfo> select() = 0;

    virtual void connect() = 0;
    virtual void createTable() = 0;
};

#endif // DAOLOGININFO_H