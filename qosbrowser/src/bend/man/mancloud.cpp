#include "mancloud.h"
#include <plugins/manplugin.h>
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