#ifndef DAOLOGININFOSQLITE_H
#define DAOLOGININFOSQLITE_H

#include "middle/models/dbmodels.h"
#include "helper/dbsqlite.h"
#include "daologininfo.h"

class DaoLoginInfoSqlite : public DaoLoginInfo
{
public:
    DaoLoginInfoSqlite();

    //对登录的信息进行操作
    bool exists(const QString& secret_id);
    void insert(const LoginInfo& info);
    void update(const LoginInfo& info);
    void remove(const QString &secretId);

    //缓存查询用
    QList<LoginInfo> select();

    //连接数据库以及创建表
    void connect();
    void createTable();
private:
    //获取表名
    const QString table_;
    DbSqlite db_;
};

#endif // DAOLOGININFOSQLITE_H
