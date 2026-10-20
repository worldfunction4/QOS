#ifndef MANCLOUD_H
#define MANCLOUD_H

#include "bend/dao/clouds/daoclouds.h"
#include <QObject>
#include <QStandardItemModel>

class ManCloud : public QObject
{
    Q_OBJECT
public:
    // 构造函数需要公有，供 Q_GLOBAL_STATIC 创建单例
    explicit ManCloud(QObject* parent=nullptr);
    // 获取单例指针
    static ManCloud* instance();
    // 从数据源读取存储桶，填充到 model
    void setManBucket();

    // 获取私有的model
    QStandardItemModel* model()const;

    // 禁用拷贝与赋值
    ManCloud(const ManCloud&) = delete;
    ManCloud& operator=(const ManCloud&) = delete;
private:
    QStandardItemModel* model_ = nullptr;
    // 云存储接口，具体实现可以是 Mock 或真实的云
    DaoClouds* dao_ = nullptr;
};

#endif // MANCLOUD_H
