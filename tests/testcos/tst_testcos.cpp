#include "tst_testcos.h"
#include "config/exceptions.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

//加俩常量进行测试用
static const QString TEST_BUCKET = "qos-test-1320342190";
static const QString TEST_REGION = "ap-chongqing";

TestCos::TestCos() {}

TestCos::~TestCos() = default;

void TestCos::initTestCase()
{
    //添加密钥
    QFile file(QString(TESTCOS_DIR) + "/secret.json");
    if(!file.open(QIODevice::ReadOnly))
        QSKIP("没有secret.json文件，跳过COS测试");
    //使用json对象读取密钥
     QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
    secretId_= obj["secretId"].toString();
    secretKey_ = obj["secretKey"].toString();

    //测试登录
    cos_.login(secretId_, secretKey_);
}

void TestCos::init()
{
    // code to be executed before each test function
}

void TestCos::cleanupTestCase()
{
    //兜底处理，测试失败过程清除测试桶
    try
    {
        cos_.deleteBucket(TEST_BUCKET, TEST_REGION);
    }
    catch(...)
    {}
}

void TestCos::cleanup()
{
    // code to be executed after each test function
}

void TestCos::test_buckets()
{
    QList<Bucket> list = cos_.buckets();
    //至少要有一个存储桶
    QVERIFY(list.size() > 0);
}

void TestCos::test_loginError()
{
    //新建，和cos_保持独立
    DaoCloudsCos cos;
    //写一个错误的key，看看能否被正确处理
    QVERIFY_THROWS_EXCEPTION(BaseException, cos.login(secretId_, "wrong_key"));
}

void TestCos::test_putBucket()
{
    cos_.putBucket(TEST_BUCKET, TEST_REGION);
    QVERIFY(cos_.isBucketExists(TEST_BUCKET));
}

void TestCos::test_putBucketAgain()
{
    //重复创建测试，应该是成功
    cos_.putBucket(TEST_BUCKET, TEST_REGION);
}

void TestCos::test_deleteBucket()
{
    cos_.deleteBucket(TEST_BUCKET, TEST_REGION);
    QVERIFY(!cos_.isBucketExists(TEST_BUCKET));
    //然后再次进行删除操作，也应该会成功
    cos_.deleteBucket(TEST_BUCKET, TEST_REGION);
}

void TestCos::test_isBucketExists()
{
    //已有存储桶存储在？
    QVERIFY(cos_.isBucketExists("file-2026-1320342190"));
    //硬编码一个随机值应当不存在
    QVERIFY(!cos_.isBucketExists("file-exist-1320342199"));
}
