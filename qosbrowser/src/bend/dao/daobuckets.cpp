#include "daobuckets.h"
#include "src/helper/filehelper.h"
#include <QDebug>

DaoBuckets::DaoBuckets() {}

QList<Bucket> DaoBuckets::bucketFromMock(const QString &path)
{
    QList<Bucket> res;
    QVariant var = FileHelper::readAllJson(path);
    QJsonArray arrs = var.toJsonArray();
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
