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

bool DaoCloudsCos::isBucketExists(const QString &bucketName)
{
    for(const auto& bucket : buckets())
    {
        if(bucket.name == bucketName)return true;
    }
    return false;
}

qcloud_cos::CosConfig DaoCloudsCos::makeConfig(const QString &region)
{
    qcloud_cos::CosConfig config;
    config.SetAccessKey(secretId_.toStdString());
    config.SetSecretKey(secretKey_.toStdString());
    config.SetRegion(region.toStdString());
    return config;
}

QList<CloudObject> DaoCloudsCos::dirList(qcloud_cos::GetBucketResp &resp, const QString &dir)
{
    QList<CloudObject> res;
    //CommonPrefixes就是被 '/' 折叠起来的子目录
    for(const std::string& prefix : resp.GetCommonPrefixes())
    {
        CloudObject obj;
        obj.dir = dir;
        //mid截取函数，比如mid(5)代表从第5个字符开始截取
        //比如 "test/bll/" 就是mid(5)，得到 "bll/"
        obj.name = QString::fromStdString(prefix).mid(dir.size());
        obj.lastModified = "-";
        res.append(obj);
    }
    return res;
}

QList<CloudObject> DaoCloudsCos::fileList(qcloud_cos::GetBucketResp &resp, const QString &dir)
{
    QList<CloudObject> res;
    for(const qcloud_cos::Content& content : resp.GetContents())
    {
        //拿到完整对象名
        QString key = QString::fromStdString(content.m_key);
        //在控制台建目录会生成一个和目录同名的空对象，跳过它
        if(key == dir)
            continue;
        CloudObject obj;
        obj.dir = dir;
        obj.name = key.mid(dir.size());
        //COS返回的字符串
        obj.lastModified = QString::fromStdString(content.m_last_modified);
        obj.size = QString::fromStdString(content.m_size).toULongLong();
        res.append(obj);
    }
    return res;
}

void DaoCloudsCos::putBucket(const QString &bucketName, const QString &location)
{
    //直接创建，不先查是否存在，只发一次请求
    //CosAPI 的构造函数要非const引用，不能直接传临时对象
    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);
    qcloud_cos::PutBucketReq req(bucketName.toStdString());
    qcloud_cos::PutBucketResp resp;
    qcloud_cos::CosResult result = cos.PutBucket(req, &resp);
    //已经是自己的桶，当作创建成功
    if(result.IsSucc() || result.GetErrorCode() == "BucketAlreadyOwnedByYou") return;
    throw BaseException(EC_331100, QString::fromStdString(result.GetErrorMsg()));
}

void DaoCloudsCos::deleteBucket(const QString &bucketName, const QString &location)
{
    //地域由调用方传入，来自桶列表，只发一次请求
    //CosAPI 的构造函数要非const引用，不能直接传临时对象
    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);
    qcloud_cos::DeleteBucketReq req(bucketName.toStdString());
    qcloud_cos::DeleteBucketResp resp;
    qcloud_cos::CosResult result = cos.DeleteBucket(req, &resp);
    //桶本来就不存在，当作删除成功，其他错误照常抛出
    if(result.IsSucc() || result.GetErrorCode() == "NoSuchBucket") return;
    throw BaseException(EC_331300, QString::fromStdString(result.GetErrorMsg()));
}

QList<CloudObject> DaoCloudsCos::getObjects(const QString &bucketName, const QString &location, const QString &dir)
{
    qcloud_cos::GetBucketReq req(bucketName.toStdString());
    //列出当前这一行
    if(!dir.isEmpty())
    {
        req.SetPrefix(dir.toStdString());
    }
    //如果后面还有'/'就折叠起来
    req.SetDelimiter("/");

    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);
    qcloud_cos::GetBucketResp resp;
    qcloud_cos::CosResult result = cos.GetBucket(req, &resp);
    if(!result.IsSucc())
    {
        throw BaseException(EC_331200, QString::fromStdString(result.GetErrorMsg()));
    }
    return dirList(resp, dir) + fileList(resp, dir);
}

bool DaoCloudsCos::isObjectExists(const QString &bucketName, const QString &location, const QString &key)
{
    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);
    return cos.IsObjectExist(bucketName.toStdString(), key.toStdString());
}

void DaoCloudsCos::putObject(const QString &bucketName, const QString &location, const QString &key, const QString &localPath, const TransProgressCallBack &callback)
{
    //异步上传请求:桶，云端key，本地文件
    qcloud_cos::AsyncPutObjectReq req(bucketName.toStdString(),
                                      key.toStdString(),
                                      localPath.toLocal8Bit().toStdString());  //本地路径先转为 win 的本地编码（GBK），修复中文路径 SDK 打不开的问题

    //调用方传回调才会进行设置
    if(callback)
    {
        req.SetTransferProgressCallback(callback);
    }
    //SDK客户端对象
    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);

    //发起上传，并等它传完，当前在工作线程里，阻塞没关系
    qcloud_cos::SharedAsyncContext context = cos.AsyncPutObject(req);
    context->WaitUntilFinish();

    qcloud_cos::CosResult result = context->GetResult();
    if (!result.IsSucc())
    {
        throw BaseException(EC_332400,QString::fromStdString(result.GetErrorMsg()));
    }

}

void DaoCloudsCos::getObject(const QString &bucketName, const QString &location, const QString &key, const QString &localPath, const TransProgressCallBack &callback)
{
    //前面创建config、CosAPI的写法和putObject一样
    qcloud_cos::AsyncGetObjectReq req(bucketName.toStdString(),
                                      key.toStdString(),
                                      localPath.toLocal8Bit().toStdString());  //本地路径先转为 win 的本地编码（GBK），修复中文路径 SDK 打不开的问题

    if (callback)
    {
        req.SetTransferProgressCallback(callback);
    }
    //SDK客户端对象
    qcloud_cos::CosConfig config = makeConfig(location);
    qcloud_cos::CosAPI cos(config);

    qcloud_cos::SharedAsyncContext context = cos.AsyncGetObject(req);
    context->WaitUntilFinish();
    if (!context->GetResult().IsSucc())
    {
        throw BaseException(EC_332500, QString::fromStdString(context->GetResult().GetErrorMsg()));
    }

}