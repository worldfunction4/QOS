#include "daocloudsmock.h"
#include "config/errorcode.h"
#include "config/exceptions.h"
#include "src/helper/filehelper.h"
#include <QJsonArray>

DaoCloudsMock::DaoCloudsMock(const QString& path)
{
    mock_ = FileHelper::readAllJson(path).toJsonValue();
}

QList<Bucket> DaoCloudsMock::buckets()
{
    QList<Bucket> res;
    QJsonArray arrs = mock_["buckets"].toArray();
    for(const QJsonValue& value: arrs)
    {
        Bucket bucket;
        bucket.name = value["name"].toString();
        bucket.location = value["location"].toString();
        bucket.createDate = value["create_date"].toString();
        res.append(bucket);
        qDebug() << bucket.name
                 << bucket.location
                 << bucket.createDate;
    }
    return res;
}

QList<Bucket> DaoCloudsMock::login(const QString& secretId, const QString& secretKey)
{
    QJsonArray users = mock_["users"].toArray();
    for(const QJsonValue& u : users)
    {
        if(secretId == u["secretId"].toString() && secretKey == u["secretKey"].toString())
            return buckets();          //找到匹配成功
    }
    throw BaseException(EC_211100, "SecretId 或 SecretKey 不正确");  //都不匹配
}

bool DaoCloudsMock::isBucketExists(const QString &bucketName)
{
    return false;
}

void DaoCloudsMock::putBucket(const QString &bucketName, const QString &location)
{
    return;
}

void DaoCloudsMock::deleteBucket(const QString &bucketName, const QString &location)
{
    return;
}

QList<CloudObject> DaoCloudsMock::getObjects(const QString &bucketName, const QString &location, const QString &dir)
{
    QList<CloudObject> res;
    return res;
}
