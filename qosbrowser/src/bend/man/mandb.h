#ifndef MANDB_H
#define MANDB_H

#include "bend/dao/dbs/daologininfo.h"
#include <QObject>



class ManDB : public QObject
{
    Q_OBJECT
public:
    explicit ManDB(QObject *parent = nullptr);
    void init();

    //保存登录信息
    void saveLoginInfo(const QString& name, const QString& id, const QString& key, const QString& remark);
    //删除登录信息
    void removeLoginInfo(const QString& id);
    //一个索引连接数据
    int logininfoIndex(const QString& secretId);
    //提供两个查询接口查看数据库存在的登录状态
    //拿到登录名列表
    QStringList loginInfoNames()const;
    //根据选择的登录名填充相应信息
    bool loginInfoByName(const QString& name, LoginInfo* info)const;

signals:
private:
    //保留接口
    DaoLoginInfo* daologininfo_ = nullptr;
    //读取登录信息到缓存，创建一个缓存
    QList<LoginInfo> logininfo_list_;
};

#endif // MANDB_H
