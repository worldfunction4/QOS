#ifndef MANDB_H
#define MANDB_H

#include "helper/dbsqlite.h"
#include <QObject>


class ManDB : public QObject
{
    Q_OBJECT
public:
    explicit ManDB(QObject *parent = nullptr);
    static ManDB* instance();
    void init();
signals:
private:
    void connect();
    DbSqlite db_;
};

#endif // MANDB_H
