#ifndef DAOCLOUDS_H
#define DAOCLOUDS_H

#include <QList>
#include "src/middle/models/bucket.h"
#include "src/middle/models/cloudobject.h"

#include <functional>

//使用回调函数
using TransProgressCallBack = std::function<void(uint64_t trans_size, uint64_t total_size, void*)>;


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
    //判断桶中是否存在某个对象？key为完整路径
    virtual bool isObjectExists(const QString& bucketName,
                                const QString& location,
                                const QString& key) = 0;
    //上传本地文件到桶里，key为完整路径
    virtual void putObject(const QString& bucketName,
                           const QString& location,
                           const QString& key,
                           const QString& localPath,
                           const TransProgressCallBack& callback)=0;

    //与上传对应，也需要下载函数,callback默认不需要进度
    virtual void getObject(const QString& bucketName, const QString& location,
                           const QString& key, const QString& localPath,
                           const TransProgressCallBack& callback=nullptr) = 0;
};

#endif // DAOCLOUDS_H
