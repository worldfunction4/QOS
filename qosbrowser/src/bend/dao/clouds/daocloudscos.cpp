#include "daocloudscos.h"
#include "config/errorcode.h"
#include "config/exceptions.h"
#include "cos_api.h"
#include "cos_sys_config.h"

DaoCloudsCos::DaoCloudsCos()
{
    //关掉SDK日志以免造成XML读取失败以及账号密码安全
    qcloud_cos::CosSysConfig::SetLogOutType(qcloud_cos::COS_LOG_NULL);
}

QList<Bucket> DaoCloudsCos::buckets()
{
    //请求
    qcloud_cos::GetServiceReq req;
    qcloud_cos::GetServiceResp resp;

    //配置
    qcloud_cos::CosConfig config;
    config.SetAccessKey(secretId_.toStdString());
    config.SetSecretKey(secretKey_.toStdString());
    //硬编码一个地域信息通过测试
    config.SetRegion("ap-guangzhou");

    //创建请求
    qcloud_cos::CosAPI cos(config);
    qcloud_cos::CosResult result = cos.GetService(req, &resp);

    //判断
    if(!result.IsSucc())
    {
        throw BaseException(EC_211100, QString::fromStdString(result.GetErrorMsg()));
    }
    //结果
    //把结果转为自己的存入临时变量并返回副本
    auto buckets = resp.GetBuckets();
    QList<Bucket> resBuckets;
    for(const auto& bucket : buckets)
    {
        Bucket tmp;
        tmp.name = QString::fromStdString(bucket.m_name);
        tmp.location = QString::fromStdString(bucket.m_location);
        tmp.createDate = QString::fromStdString(bucket.m_create_date);
        resBuckets.append(tmp);
    }
    return resBuckets;
}

QList<Bucket> DaoCloudsCos::login(const QString &secretId, const QString &secretKey)
{
    //密钥存入成员变量
    secretId_ = secretId;
    secretKey_ = secretKey;

    //返回buckets
    return buckets();
}
