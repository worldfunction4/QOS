#ifndef DAOCLOUDS_H
#define DAOCLOUDS_H

#include <QList>
#include "src/middle/models/bucket.h"

//对云的接口进行抽象
class DaoClouds
{
public:
    DaoClouds()=default;
    //获取存储桶的抽象，所有方式都从这里重写
    virtual QList<Bucket> buckets() = 0;
};

#endif // DAOCLOUDS_H
