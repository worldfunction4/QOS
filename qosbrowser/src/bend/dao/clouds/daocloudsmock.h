#ifndef DAOCLOUDSMOCK_H
#define DAOCLOUDSMOCK_H

#include "daoclouds.h"

#include <QJsonValue>

class DaoCloudsMock : public DaoClouds
{
public:
    DaoCloudsMock(const QString& path);
    QList<Bucket> buckets()override;
    QList<Bucket> login(const QString& secretId, const QString& secretKey) override;

    bool isBucketExists(const QString& bucketName)override;
    void putBucket(const QString& bucketName, const QString& location)override;
    void deleteBucket(const QString& bucketName, const QString& location)override;
private:
    //mock路径
    QJsonValue mock_;
};

#endif // DAOCLOUDSMOCK_H
