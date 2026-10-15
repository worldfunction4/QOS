#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QString>
#include <QSqlDatabase>

class DbSqlite
{
public:
    DbSqlite();
    ~DbSqlite();
    void connect(const QString& path);

private:
    //创建数据库对象
    QSqlDatabase sqldata_;
};

#endif // DBSQLITE_H