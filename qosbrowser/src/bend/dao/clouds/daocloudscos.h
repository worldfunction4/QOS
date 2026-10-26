#ifndef DAOCLOUDSCOS_H
#define DAOCLOUDSCOS_H

#include "daoclouds.h"
#include <QString>

namespace qcloud_cos { class CosConfig; }

class DaoCloudsCos : public DaoClouds
{
public:
    DaoCloudsCos();
    QList<Bucket> buckets()override;
    QList<Bucket> login(const QString& secretId,
                        const QString& secretKey) override;

    bool isBucketExists(const QString& bucketName)override;
    void putBucket(const QString& bucketName, const QString& location)override;
    void deleteBucket(const QString& bucketName, const QString& location)override;
private:
    //生成访问配置，密钥和地域统一在这里设置
    qcloud_cos::CosConfig makeConfig(const QString& region);
    QString secretId_;
    QString secretKey_;
};

#endif // DAOCLOUDSCOS_H
