#include "gateway.h"
#include "config/exceptions.h"
#include "config/errorcode.h"
#include "config/loggerproxy.h"
#include "middle/signals/mansignals.h"
#include "config/apis.h"
#include "middle/manglobal.h"

#include <QtConcurrent>
#include <QString>

Gateway::Gateway()
{}

void Gateway::send(int api, const QJsonValue &value)
{
    //放进线程池，然后自己直接返回,(void)告诉编译器就是故意不等结果的
    (void)QtConcurrent::run([=]{
        //子线程里，调用dispatch，传入相应的api和参数
        try
        {
            dispatch(api, value);
        }
        catch(const BaseException& e)
        {
            //得到BaseException类型的异常。生成日志，发送error信号
            MERROR(e.msg());
            emit MG->mSignal->error(api, e.msg());
        }
        catch(...)
        {
            //如果是未知异常使用BaseException兜底，生成日志，发送error信号
            BaseException e = BaseException(EC_100000, "未知错误");
            MERROR(e.msg());
            emit MG->mSignal->error(api, e.msg());
        }
    });
}

void Gateway::dispatch(int api, const QJsonValue &value)
{
    //使用switch语句进行api选择调用函数
    switch(api)
    {
        case API::LOGIN::NORMAL:
            apiLogin(value);
            break;
        //误匹配接口函数抛出异常，防止界面一直等死
        default:
            throw BaseException(EC_100000, "未知接口，无法找到对应处理函数");
    }
}

void Gateway::apiLogin(const QJsonValue &value)
{
    //dispatch调用。拿到value进行取值
    QString secretid = value["secretId"].toString();
    QString secretkey = value["secretKey"].toString();
    //检查：如果为空抛出异常
    if(secretid.isEmpty() || secretkey.isEmpty())
    {
        throw BaseException(EC_211100, "账号或密码为空，请重新输入");
    }
    //TODO:接入ManCloud登录腾讯云,当前没有校验账号密码，只要不为空就能通过


    emit MG->mSignal->loginSuccess();
}
