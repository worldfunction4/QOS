#ifndef BUCKET_H
#define BUCKET_H

#include <QString>
#include <QObject>

//纯数据
struct Bucket
{
    QString name;
    QString location;
    QString createDate;

    //显示要求让编译器生成下面四个函数
    Bucket() = default;
    Bucket(const Bucket&) = default;
    Bucket& operator=(const Bucket&) = default;
    ~Bucket() = default;
};

//编译时让qt认识此类型，可以复制，放入qvariant，排队传递等
//qt6可以省略
Q_DECLARE_METATYPE(Bucket)

#endif // BUCKET_H
