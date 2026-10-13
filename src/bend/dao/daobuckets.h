#ifndef DAOBUCKETS_H
#define DAOBUCKETS_H

#include "src/bend/models/bucket.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QJsonArray>

class DaoBuckets
{
public:
    DaoBuckets();

    QList<Bucket> bucketFromMock(const QString& path);
};

#endif // DAOBUCKETS_H
