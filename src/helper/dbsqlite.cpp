#include "dbsqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

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

QList<Record> DbSqlite::select(const QString& sql)
{
    QList<Record> ret_list;
    QSqlQuery query;

    //执行sql语句
    if(!query.exec(sql))
    {
        qDebug() << "SQl执行失败:" << query.lastError().text();
        return ret_list;
    }
    //获取结果信息
    QSqlRecord record = query.record();
    int column_count = record.count();

    while(query.next())
    {
        Record row_map;
        for(int i=0; i < column_count; ++i)
        {
            QString field_name = record.fieldName(i);
            QVariant field_value = query.value(i);
            row_map.insert(field_name, field_value);
        }
        //加入列表
        ret_list.append(row_map);
    }
    return ret_list;
}
