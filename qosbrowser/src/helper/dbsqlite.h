#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QString>
#include <QSqlDatabase>
#include <QMap>
#include <QList>

using Record = QMap<QString, QVariant>;

class DbSqlite
{
public:
    DbSqlite();
    ~DbSqlite();
    //连接数据库
    void connect(const QString& path);
    //执行sql语句
    void exec(const QString& sql);
    //执行select函数
    QList<Record> select(const QString& sql);

private:
    //创建数据库对象
    QSqlDatabase sqldata_;
};

#endif // DBSQLITE_H