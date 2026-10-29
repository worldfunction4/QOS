#include "mancloud.h"
#include <plugins/manplugin.h>
#include "config/errorcode.h"
#include "config/exceptions.h"
#include "middle/manglobal.h"
#include "middle/signals/mansignals.h"
#include "src/helper/bytehelper.h"

#include <QMetaType>
#include <QVariant>
#include <QList>
#include <QDateTime>

//把云端的 UTC 时间转成本地时间，转不了就原样返回
static QString toLocalTimeStr(const QString& utc)
{
    //按 ISO 格式解析，Z 结尾会被认作 UTC
    QDateTime dt = QDateTime::fromString(utc, Qt::ISODate);
    if (!dt.isValid())
        return utc;
    //转成电脑本地时区，再换成好读的格式
    return dt.toLocalTime().toString("yyyy-MM-dd hh:mm:ss");
}

ManCloud::ManCloud(QObject* parent)
    :QObject(parent)
{
    modelBuckets_ = new QStandardItemModel(this);
    modelObjects_ = new QStandardItemModel(this);

    //排序不看显示文字
    modelObjects_->setSortRole(Qt::UserRole + 1);


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
        modelBuckets_->setData(index2, toLocalTimeStr(bucket.createDate));

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
        //名称
        QModelIndex index0 = modelObjects_->index(row, 0);
        modelObjects_->setData(index0, object.name);
        modelObjects_->setData(index0, QVariant::fromValue(object), Qt::UserRole);

        //排序依据
        modelObjects_->setData(index0, object.name, Qt::UserRole + 1);

        //大小
        QModelIndex index1 = modelObjects_->index(row, 1);
        modelObjects_->setData(index1, object.isDir()? QString("-") : ByteHelper::toBeautifulStr(object.size));

        //排序采取原始字节数
        modelObjects_->setData(index1, object.isDir()? int64_t(-1) : int64_t(object.size), Qt::UserRole + 1);

        //修改时间
        QModelIndex index2 = modelObjects_->index(row, 2);
        modelObjects_->setData(index2, toLocalTimeStr(object.lastModified));
        //排序依据：修改时间
        modelObjects_->setData(index2, toLocalTimeStr(object.lastModified), Qt::UserRole + 1);

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
    bucketsAlready(buckets);
}

void ManCloud::onUnLogin()
{
    //删除数据行
    modelBuckets_->setRowCount(0);
    modelObjects_->setRowCount(0);

    //清空当前位置
    currentBucket_.clear();
    currentLocation_.clear();
    currentDir_.clear();

    //清除dao
    dao_ = nullptr;
}

void ManCloud::getBuckets()
{
    bucketsAlready(dao()->buckets());
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
    //取数据，失败抛出异常
    QList<CloudObject> objects = dao()->getObjects(bucketName, location, dir);

    //然后记住当前位置
    currentBucket_ = bucketName;
    currentLocation_ = location;
    currentDir_ = dir;

    //然后再发信号通知界面
    emit MG->mSignal->objectsSuccess(objects);
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

QString ManCloud::currentBucket() const
{
    return currentBucket_;
}

QString ManCloud::currentLocation() const
{
    return currentLocation_;
}

QString ManCloud::currentDir() const
{
    return currentDir_;
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

void ManCloud::bucketsAlready(const QList<Bucket> &buckets)
{
    //回到桶列表，就不在任何桶里了
    currentBucket_.clear();
    currentLocation_.clear();
    currentDir_.clear();

    emit MG->mSignal->bucketsSuccess(buckets);
}
