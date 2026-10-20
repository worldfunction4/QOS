#include "daocloudsmock.h"
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
