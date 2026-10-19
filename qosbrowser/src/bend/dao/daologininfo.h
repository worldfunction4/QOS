#ifndef DAOLOGININFO_H
#define DAOLOGININFO_H

#include "middle/models/dbmodels.h"
#include "helper/dbsqlite.h"

class DaoLoginInfo
{
public:
    DaoLoginInfo();

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

#endif // DAOLOGININFO_H
