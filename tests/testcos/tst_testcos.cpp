#include "bend/dao/clouds/daocloudscos.h"
#include "config/exceptions.h"
//关闭SDK日志
#include "cos_sys_config.h"
#include <QTest>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>


// add necessary includes here

class TestCos : public QObject
{
    Q_OBJECT

public:
    TestCos();
    ~TestCos() override;

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_buckets();
    void test_loginError();

private:
    DaoCloudsCos cos_;
    QString secretId_;
    QString secretKey_;
};

TestCos::TestCos() {}

TestCos::~TestCos() = default;

void TestCos::initTestCase()
{
    //把SDK的日志关闭
    qcloud_cos::CosSysConfig::SetLogOutType(qcloud_cos::COS_LOG_NULL);
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
    // code to be executed after the last test function
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
    QVERIFY_EXCEPTION_THROWN(cos.login(secretId_, "wrong_key"),BaseException);
}

QTEST_APPLESS_MAIN(TestCos)

#include "tst_testcos.moc"
