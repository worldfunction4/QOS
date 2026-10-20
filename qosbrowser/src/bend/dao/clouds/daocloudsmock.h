#ifndef DAOCLOUDSMOCK_H
#define DAOCLOUDSMOCK_H

#include "daoclouds.h"

#include <QJsonValue>

class DaoCloudsMock : public DaoClouds
{
public:
    DaoCloudsMock(const QString& path);
    QList<Bucket> buckets()override;
private:
    //mock路径
    QJsonValue mock_;
};

#endif // DAOCLOUDSMOCK_H
