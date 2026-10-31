#include "gateway.h"
#include "bend/man/mancloud.h"
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
    //放进线程池，然后自己直接返回,(void)告诉编译器就是不等结果
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
            emit MG->mSignal->error(api, e.msg(), value);
        }
        //出现位置错误了...
        catch(const std::exception& e)
        {
            BaseException ex(EC_100000, QString::fromUtf8(e.what()));
            MERROR(ex.msg());
            emit MG->mSignal->error(api, ex.msg(), value);
        }
        catch(...)
        {
            //如果是未知异常使用BaseException兜底，生成日志，发送error信号
            BaseException e = BaseException(EC_100000, "未知错误");
            MERROR(e.msg());
            emit MG->mSignal->error(api, e.msg(), value);
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
        //桶
        case API::BUCKET::LIST:
            apiGetBucket(value);
            break;
        case API::BUCKET::PUT:
            apiPutBucket(value);
            break;
        case API::BUCKET::DEL:
            apiDeleteBucket(value);
            break;
        //对象
        case API::OBJECT::LIST:
            apiGetObjcets(value);
            break;
        case API::OBJECT::GET:
            apiGetObjcet(value);
            break;
        case API::OBJECT::PUT:
            apiPutObject(value);
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

    MG->mCloud->login(secretid, secretkey);
    emit MG->mSignal->loginSuccess();
    //TODO:接入ManCloud登录腾讯云,当前没有校验账号密码，只要不为空就能通过

}

void Gateway::apiGetBucket(const QJsonValue &value)
{
    MG->mCloud->getBuckets();
}

void Gateway::apiPutBucket(const QJsonValue &value)
{
    MG->mCloud->putBucket(value["bucketName"].toString(),
                          value["location"].toString());
}

void Gateway::apiDeleteBucket(const QJsonValue &value)
{
    MG->mCloud->deleteBucket(value["bucketName"].toString(),
                             value["location"].toString());
}

void Gateway::apiGetObjcets(const QJsonValue &value)
{
    MG->mCloud->getObjects(value["bucketName"].toString(),
                           value["location"].toString(),
                           value["dir"].toString());
}

void Gateway::apiGetObjcet(const QJsonValue &value)
{
    MG->mCloud->getObject(value["workId"].toString(),
                          value["bucketName"].toString(),
                          value["location"].toString(),
                          value["key"].toString(),
                          value["localPath"].toString());
}

void Gateway::apiPutObject(const QJsonValue &value)
{
    MG->mCloud->putObject(value["workId"].toString(),
                          value["bucketName"].toString(),
                          value["location"].toString(),
                          value["key"].toString(),
                          value["localPath"].toString());
}
