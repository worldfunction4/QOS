#ifndef DBMODELS_H
#define DBMODELS_H

#include <Qstring>


//登录信息数据
struct LoginInfo
{
    QString name;
    QString secretId;
    QString secretKey;
    QString remark;
    uint timeStamp;
};


#endif // DBMODELS_H
