#include "tst_testcos.h"
#include "config/exceptions.h"
#include "helper/filehelper.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

#include <QDebug>

//加俩常量进行测试用
static const QString TEST_BUCKET = "qos-test-1320342190";
static const QString TEST_REGION = "ap-chongqing";

//列一个对象用的真实桶
static const QString OBJ_BUCKET = "file-2026-1320342190";

//上传本地临时文件，以及在云端的key
//static const QString UPLOAD_LOCAL = QDir::tempPath() + "/qos_upload_test.txt";

//上传用的本地临时文件，用到时再算路径，避免全局变量初始化时 Qt 还没准备好
static QString uploadLocal()
{
    return QString(TESTCOS_DIR) + "/qos_upload_test.txt";
}

//下载保存到本地文件，目录放在测试文件夹内
static QString downloadLocal()
{
    return QString(TESTCOS_DIR) + "/qos_download_test.txt";
}

static const QString UPLOAD_KEY   = "qos_upload_test.txt";

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

    try
    {
        //尝试登录
        cos_.login(secretId_, secretKey_);
        //打印路径
        qDebug() << "文件上传的路径:" << uploadLocal();
        //准备上传用的本地文件
        FileHelper::writeFile(QStringList() << "abc" << "def", uploadLocal());
    }
    catch (const BaseException& e)
    {
        QFAIL(qPrintable(e.msg()));
    }
    catch (const char* s)
    {
        //FileHelper 目前抛的是字符串，先在这里接住
        QFAIL(qPrintable(QString::fromUtf8(s)));
    }
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

    //清除
    QFile::remove(uploadLocal());

    //清理下载产生的文件
    QFile::remove(downloadLocal());
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
    try
    {
        cos_.putBucket(TEST_BUCKET, TEST_REGION);
        QVERIFY(cos_.isBucketExists(TEST_BUCKET));
    }
    catch (const BaseException& e)
    {
        //把错误码和云端返回的信息打印到测试报告里
        QFAIL(qPrintable(e.msg()));
    }
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

void TestCos::test_getObjects_data()
{
    QTest::addColumn<QString>("dir");
    QTest::addColumn<int>("expected");

    //一行为一组用例
    QTest::newRow("root") << "" << 5;
    //如果有子文件夹
    //QTest::newRow("subdir") << "test/" << 0;

}

void TestCos::test_getObjects()
{
    QFETCH(QString, dir);
    QFETCH(int, expected);

    QList<CloudObject> list = cos_.getObjects(OBJ_BUCKET, TEST_REGION, dir);
    QCOMPARE(list.size(), expected);

    //每一个对象都不能为空名字，需要属于此目录
    for(const CloudObject& obj : list)
    {
        QVERIFY(!obj.name.isEmpty());
        QCOMPARE(obj.dir, dir);
    }
}

void TestCos::test_getObjectError()
{
    //访问不存在的桶，getObjects应该抛出 BaseException（错误码 331200）
    QVERIFY_THROWS_EXCEPTION(BaseException,
                             cos_.getObjects("not-exist-1320342190", TEST_REGION, ""));
}

void TestCos::test_putObject()
{
    try
    {
        //回调传空
        cos_.putObject(OBJ_BUCKET, TEST_REGION, UPLOAD_KEY, uploadLocal(), nullptr);
        QVERIFY(cos_.isObjectExists(OBJ_BUCKET, TEST_REGION, UPLOAD_KEY));
    }
    catch(const BaseException& e)
    {
        QFAIL(qPrintable(e.msg()));
    }

}

void TestCos::test_getObject()
{
    //删除上次留下的文件
    QFile::remove(downloadLocal());

    try
    {
        cos_.getObject(OBJ_BUCKET, TEST_REGION, UPLOAD_KEY, downloadLocal());
        QVERIFY(QFile::exists(downloadLocal()));
    }
    catch(const BaseException& e)
    {
        QFAIL(qPrintable(e.msg()));
    }

}