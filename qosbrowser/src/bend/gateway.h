#ifndef GATEWAY_H
#define GATEWAY_H

#include <QJsonValue>

class Gateway
{
public:
    Gateway();
    //声明单例
    static Gateway* instance();

    //界面调用
    void send(int api, const QJsonValue& value);

private:
    void dispatch(int api, const QJsonValue& value); //路由表
    void apiLogin(const QJsonValue& value); //登录模块
};

#endif // GATEWAY_H
