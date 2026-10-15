#include "mandb.h"
#include "src/config/config.h"
Q_GLOBAL_STATIC(ManDB, ins)


ManDB::ManDB(QObject *parent)
    : QObject{parent}
{}

ManDB *ManDB::instance()
{
    return ins();
}

void ManDB::init()
{
    connect();
    creatLoginInfoTable();
}

void ManDB::connect()
{
    //取消硬编码
    db_.connect(CONFIG::SQLITE::NAME);
}

void ManDB::creatLoginInfoTable()
{
    //创建表
    QString sql = FileHelper::readAllText(CONFIG::SQL::LOGIN_INFO_PATH);
    db_.exec(sql);
}
