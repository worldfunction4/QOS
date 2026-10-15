#ifndef MANDB_H
#define MANDB_H


#include "bend/dao/daologininfo.h"
#include <QObject>



class ManDB : public QObject
{
    Q_OBJECT
public:
    explicit ManDB(QObject *parent = nullptr);
    static ManDB* instance();
    void init();

    //保存登录信息
    void saveLoginInfo(const QString& name, const QString& id, const QString& key, const QString& remark);
    //删除登录信息
    void removeLoginInfo(const QString& id);

signals:
private:
    DaoLoginInfo daologininfo_;
};

#endif // MANDB_H
