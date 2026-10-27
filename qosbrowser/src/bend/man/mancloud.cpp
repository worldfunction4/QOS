#include "mancloud.h"
#include <plugins/manplugin.h>
#include "config/errorcode.h"
#include "config/exceptions.h"
#include "middle/manglobal.h"
#include "middle/signals/mansignals.h"

#include <QMetaType>

ManCloud::ManCloud(QObject* parent)
    :QObject(parent)
{
    model_ = new QStandardItemModel(this);
    //运行时只注册元类型系统，排队连接才能复制这个参数
    //qt6可以省略
    qRegisterMetaType<QList<Bucket>>("QList<Bucket>");
}

void ManCloud::setManBucket(const QList<Bucket>& buckets)
{

    model_->setRowCount(buckets.size());
    // 设置列数：名称、地域、创建时间
    model_->setColumnCount(3);
    // 行计数器
    int row = 0;
    // 遍历存储桶，逐行写入 model
    for(const Bucket& bucket: buckets)
    {
        QModelIndex index0 = model_->index(row, 0);
        model_->setData(index0, bucket.name);
        model_->setData(index0, QString("存储桶名称：%1").arg(bucket.name), Qt::ToolTipRole);

        QModelIndex index1 = model_->index(row, 1);
        model_->setData(index1, bucket.location);

        QModelIndex index2 = model_->index(row, 2);
        model_->setData(index2, bucket.createDate);

        ++row;
    }
    // 按创建时间降序排列
    model_->sort(2, Qt::DescendingOrder);
}

QStandardItemModel *ManCloud::model() const
{
    return model_;
}

void ManCloud::login(const QString &secretId, const QString &secretKey)
{
    dao_ = MG->mPlugin->clouds();
    QList<Bucket> buckets = dao_->login(secretId, secretKey);
    emit MG->mSignal->bucketsSuccess(buckets);
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