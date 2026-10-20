#include "mancloud.h"
#include <bend/dao/clouds/daocloudsmock.h>

Q_GLOBAL_STATIC(ManCloud, ins);

ManCloud::ManCloud(QObject* parent)
    :QObject(parent)
{
    // 确保没有父对象，防止生命周期被外部接管
    Q_ASSERT(parent == nullptr);

    model_ = new QStandardItemModel(this);
    // 目前使用 Mock 数据源
    dao_ = new DaoCloudsMock(":/resource/static/tests/buckets.json");
}

ManCloud *ManCloud::instance()
{
    // 返回 Q_GLOBAL_STATIC 内部创建的单例指针
    return ins();
}

void ManCloud::setManBucket()
{
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