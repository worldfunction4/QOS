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
    QList<CloudObject> getObjects(const QString& bucketName,
                                 const QString& location,
                                 const QString& dir)override;

    bool isObjectExists(const QString& bucketName,
                                const QString& location,
                                const QString& key)override;

    void putObject(const QString& bucketName,
                   const QString& location,
                   const QString& key,
                   const QString& localPath,
                   const TransProgressCallBack& callback)override;

    void getObject(const QString& bucketName, const QString& location,
                   const QString& key, const QString& localPath,
                   const TransProgressCallBack& callback=nullptr)override;

private:
    //mock路径
    QJsonValue mock_;
};

#endif // DAOCLOUDSMOCK_H
