#ifndef DAOCLOUDSCOS_H
#define DAOCLOUDSCOS_H

#include "daoclouds.h"
#include <QString>

namespace qcloud_cos
{
    class CosConfig;
    class GetBucketResp;
}

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
private:
    //生成访问配置，密钥和地域统一在这里设置
    qcloud_cos::CosConfig makeConfig(const QString& region);
    //添加辅助函数：拿到子目录和文件
    QList<CloudObject> dirList(qcloud_cos::GetBucketResp& resp, const QString& dir);
    QList<CloudObject> fileList(qcloud_cos::GetBucketResp& resp, const QString& dir);

    QString secretId_;
    QString secretKey_;
};

#endif // DAOCLOUDSCOS_H
