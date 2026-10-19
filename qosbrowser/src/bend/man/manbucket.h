#ifndef MANBUCKET_H
#define MANBUCKET_H

#include <QObject>
#include <QStandardItemModel>

class ManBucket : public QObject
{
    Q_OBJECT
public:
    //这里需公有，使用单例指针即可
    explicit ManBucket(QObject* parent=nullptr);
    //获取单例指针
    static ManBucket* instance();
    //设置测试函数
    void setManBucket();

    //获取私有的model
    QStandardItemModel* model()const;

    // 禁用拷贝与赋值
    ManBucket(const ManBucket&) = delete;
    ManBucket& operator=(const ManBucket&) = delete;
private:
    QStandardItemModel* model_ = nullptr;
};

#endif // MANBUCKET_H
