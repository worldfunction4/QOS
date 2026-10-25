#ifndef DAOCLOUDSCOS_H
#define DAOCLOUDSCOS_H

#include "daoclouds.h"
#include <QString>

class DaoCloudsCos : public DaoClouds
{
public:
    DaoCloudsCos() = default;
    QList<Bucket> buckets()override;
    QList<Bucket> login(const QString& secretId,
                        const QString& secretKey) override;

private:
    QString secretId_;
    QString secretKey_;
};

#endif // DAOCLOUDSCOS_H
