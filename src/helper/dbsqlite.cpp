#include "dbsqlite.h"
#include <QSqlError>
#include <QSqlQuery>

DbSqlite::DbSqlite()
{
    sqldata_ = QSqlDatabase::addDatabase("QSQLITE");
}

DbSqlite::~DbSqlite()
{
    //分离数据库
    if(sqldata_.isOpen())
    {
        sqldata_.close();
    }
}

void DbSqlite::connect(const QString &path)
{
    sqldata_.setDatabaseName(path);
    if(!sqldata_.open())
    {
        throw QString("数据库打开失败,数据库路径为:%1,错误信息:%2").arg(path).arg(sqldata_.lastError().text());
    }
}

void DbSqlite::exec(const QString &sql)
{
    QSqlQuery query;
    if(!query.exec(sql))
    {
        throw QString("SQL语句执行失败,SQL:%1,错误信息:%2").arg(sql).arg(query.lastError().text());
    }
}
