#include "manbucket.h"
#include "bend/dao/daobuckets.h"

Q_GLOBAL_STATIC(ManBucket, ins);

ManBucket::ManBucket(QObject* parent)
    :QObject(parent)
{
    //确保没有父对象,防止声明周期被外部接管
    Q_ASSERT(parent == nullptr);

    model_ = new QStandardItemModel(this);
}

ManBucket *ManBucket::instance()
{
    //返回Q_GLOBAL_STATIC内部创建的指针
    return ins();
}

void ManBucket::setManBucket()
{
    DaoBuckets buckets;
    QList<Bucket> bucket_lists = buckets.bucketFromMock(":/resource/static/tests/buckets.json");
    model_->setRowCount(bucket_lists.size());
    //创建列
    model_->setColumnCount(3);
    //设置计数器
    int row = 0;
    //遍历内容
    for(const Bucket& bucket: bucket_lists)
    {
        QModelIndex index0 = model_->index(row, 0);
        model_->setData(index0, bucket.name);
        model_->setData(index0, QString("存储桶名称:%1").arg(bucket.name), Qt::ToolTipRole);

        QModelIndex index1 = model_->index(row, 1);
        model_->setData(index1, bucket.location);

        QModelIndex index2 = model_->index(row, 2);
        model_->setData(index2, bucket.createDate);

        ++row;
    }
}

QStandardItemModel *ManBucket::model() const
{
    return model_;
}
