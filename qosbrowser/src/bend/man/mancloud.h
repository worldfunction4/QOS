#ifndef MANCLOUD_H
#define MANCLOUD_H

#include "bend/dao/clouds/daoclouds.h"
#include <QObject>
#include <QStandardItemModel>
#include <QList>

class ManCloud : public QObject
{
    Q_OBJECT
public:
    // 构造函数需要公有，由 ManGlobal 创建并管理
    explicit ManCloud(QObject* parent=nullptr);
    // 从数据源读取存储桶，填充到 modelBuckets
    void setBuckets(const QList<Bucket>& buckets);

    // 对象列表填充到 modelObjects
    void setObjects(const QList<CloudObject>& objects);

    // 获取私有的modelBuckets
    QStandardItemModel* modelBuckets()const;

    // 获取私有的modelObjects
    QStandardItemModel* modelObjects()const;

    //登录转发
    void login(const QString& secretId,
               const QString& secretKey);

    //退出登录时各模块清理数据
    void onUnLogin();

    //桶
    void getBuckets();
    void putBucket(const QString& bucketName, const QString& location);
    void deleteBucket(const QString& bucketName, const QString& location);

    //对象
    void getObjects(const QString& bucketName, const QString& location, const QString& dir = "");
    void getObject(const QString& workId, const QString& bucketName, const QString& location,
                   const QString& key, const QString& localPath);
    void putObject(const QString& workId, const QString& bucketName, const QString& location,
                   const QString& key, const QString& localPath);

    // 禁用拷贝与赋值
    ManCloud(const ManCloud&) = delete;
    ManCloud& operator=(const ManCloud&) = delete;
private:
    // 云存储接口，具体实现可以是 Mock 或真实的云
    DaoClouds* dao_ = nullptr;
    //桶
    QStandardItemModel* modelBuckets_ = nullptr;
    //对象
    QStandardItemModel* modelObjects_ = nullptr;
    //拿到dao,保护作用，防止没登录就调用导致直接空指针崩溃
    DaoClouds* dao()const;
};

#endif // MANCLOUD_H
