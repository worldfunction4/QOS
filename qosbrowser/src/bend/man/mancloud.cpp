#include "mancloud.h"
#include <plugins/manplugin.h>
#include "config/errorcode.h"
#include "config/exceptions.h"
#include "middle/manglobal.h"
#include "middle/signals/mansignals.h"

#include <QMetaType>
#include <QVariant>

ManCloud::ManCloud(QObject* parent)
    :QObject(parent)
{
    modelBuckets_ = new QStandardItemModel(this);
    modelObjects_ = new QStandardItemModel(this);

    //运行时只注册元类型系统，排队连接才能复制这个参数
    //qt6可以省略
    qRegisterMetaType<QList<Bucket>>("QList<Bucket>");
    qRegisterMetaType<QList<CloudObject>>("QList<CloudObject>");
}

void ManCloud::setBuckets(const QList<Bucket>& buckets)
{

    modelBuckets_->setRowCount(buckets.size());
    // 设置列数：名称、地域、创建时间
    modelBuckets_->setColumnCount(3);
    // 行计数器
    int row = 0;
    // 遍历存储桶，逐行写入 model
    for(const Bucket& bucket: buckets)
    {
        QModelIndex index0 = modelBuckets_->index(row, 0);
        modelBuckets_->setData(index0, bucket.name);
        modelBuckets_->setData(index0, QString("存储桶名称：%1").arg(bucket.name), Qt::ToolTipRole);

        QModelIndex index1 = modelBuckets_->index(row, 1);
        modelBuckets_->setData(index1, bucket.location);

        QModelIndex index2 = modelBuckets_->index(row, 2);
        modelBuckets_->setData(index2, bucket.createDate);

        ++row;
    }
    // 按创建时间降序排列
    modelBuckets_->sort(2, Qt::DescendingOrder);
}

void ManCloud::setObjects(const QList<CloudObject> &objects)
{
    // 列:名称，大小，修改时间
    modelObjects_->setRowCount(objects.size());
    modelObjects_->setColumnCount(3);

    // 行计数器
    int row = 0;

    for(const CloudObject& object : objects)
    {
        // 名称
        QModelIndex index0 = modelObjects_->index(row, 0);
        modelObjects_->setData(index0, object.name);
        modelObjects_->setData(index0, QVariant::fromValue(object), Qt::UserRole);

        // 大小
        QModelIndex index1 = modelObjects_->index(row, 1);
        modelObjects_->setData(index1, object.isDir()? QString("-") : QString::number(object.size));

        // 修改时间
        QModelIndex index2 = modelObjects_->index(row, 2);
        modelObjects_->setData(index2, object.lastModified);

        ++row;
    }
}

QStandardItemModel *ManCloud::modelBuckets() const
{
    return modelBuckets_;
}

QStandardItemModel *ManCloud::modelObjects() const
{
    return modelObjects_;
}

void ManCloud::login(const QString &secretId, const QString &secretKey)
{
    dao_ = MG->mPlugin->clouds();
    QList<Bucket> buckets = dao_->login(secretId, secretKey);
    emit MG->mSignal->bucketsSuccess(buckets);
}

void ManCloud::onUnLogin()
{
    //删除数据行
    modelBuckets_->setRowCount(0);
    modelObjects_->setRowCount(0);

    //清除dao
    dao_ = nullptr;
}

void ManCloud::getBuckets()
{
    emit MG->mSignal->bucketsSuccess(dao()->buckets());
}

void ManCloud::putBucket(const QString &bucketName, const QString &location)
{
    dao()->putBucket(bucketName, location);
    //自动刷新
    getBuckets();
}

void ManCloud::deleteBucket(const QString &bucketName, const QString &location)
{
    dao()->deleteBucket(bucketName, location);
    getBuckets();
}

void ManCloud::getObjects(const QString &bucketName, const QString &location, const QString &dir)
{
    emit MG->mSignal->objectsSuccess(dao()->getObjects(bucketName, location, dir));
}

void ManCloud::getObject(const QString &workId, const QString &bucketName, const QString &location, const QString &key, const QString &localPath)
{
    //进度回调：SDK每传一块调用一次，转成信号发给界面
    auto callback = [workId](long long transferred, long long total, void*)
    {
        emit MG->mSignal->downloadProcess(workId, transferred, total);
    };
    dao()->getObject(bucketName, location, key, localPath, callback);
    emit MG->mSignal->downloadSuccess(workId);
}

void ManCloud::putObject(const QString &workId, const QString &bucketName, const QString &location, const QString &key, const QString &localPath)
{
    auto callback = [workId](uint64_t transferred, uint64_t total, void*) {
        emit MG->mSignal->uploadProcess(workId, transferred, total);
    };
    dao()->putObject(bucketName, location, key, localPath, callback);
    emit MG->mSignal->uploadSuccess(workId);
}

DaoClouds *ManCloud::dao() const
{
    //没有dao说明还没登录
    if(!dao_)
    {
        throw BaseException(EC_100000, "需要先登录");
    }
    return dao_;
}