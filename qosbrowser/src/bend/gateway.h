#ifndef GATEWAY_H
#define GATEWAY_H

#include <QJsonValue>

class Gateway
{
public:
    Gateway();

    //界面调用
    void send(int api, const QJsonValue& value);

private:
    void dispatch(int api, const QJsonValue& value); //路由表
    void apiLogin(const QJsonValue& value); //登录模块

    //桶操作
    void apiGetBucket(const QJsonValue& value);
    void apiPutBucket(const QJsonValue& value);
    void apiDeleteBucket(const QJsonValue& value);

    //对象操作
    void apiGetObjcets(const QJsonValue& value);
    void apiGetObjcet(const QJsonValue& value);
    void apiPutObject(const QJsonValue& value);
};

#endif // GATEWAY_H
