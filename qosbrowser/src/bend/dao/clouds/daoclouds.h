#ifndef DAOCLOUDS_H
#define DAOCLOUDS_H

#include <QList>
#include "src/middle/models/bucket.h"
#include "src/middle/models/cloudobject.h"

//对云的接口进行抽象
class DaoClouds
{
public:
    DaoClouds()=default;
    virtual ~DaoClouds()=default;
    //获取存储桶的抽象，所有方式都从这里重写
    virtual QList<Bucket> buckets() = 0;
    //登录校验
    virtual QList<Bucket> login(const QString& secretId, const QString& secretKey)=0;
    //查询桶是否存在
    virtual bool isBucketExists(const QString& bucketName) = 0;
    //创建存储桶
    virtual void putBucket(const QString& bucketName, const QString& location) = 0;
    //删除存储桶
    virtual void deleteBucket(const QString& bucketName, const QString& location) = 0;
    //列出目录下子目录以及文件
    virtual QList<CloudObject> getObjects(const QString& bucketName,
                                         const QString& location,
                                         const QString& dir) = 0;
};

#endif // DAOCLOUDS_H
