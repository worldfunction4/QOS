#include "mancloud.h"
#include <plugins/manplugin.h>
#include "middle/manglobal.h"

ManCloud::ManCloud(QObject* parent)
    :QObject(parent)
{
    model_ = new QStandardItemModel(this);
}

void ManCloud::setManBucket()
{
    // 目前使用 Mock 数据源
    dao_ = MG->mPlugin->clouds();

    // 通过接口获取存储桶列表，不关心数据来自哪里
    QList<Bucket> bucket_lists = dao_->buckets();

    model_->setRowCount(bucket_lists.size());
    // 设置列数：名称、地域、创建时间
    model_->setColumnCount(3);
    // 行计数器
    int row = 0;
    // 遍历存储桶，逐行写入 model
    for(const Bucket& bucket: bucket_lists)
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
    dao_->login(secretId, secretKey);
}