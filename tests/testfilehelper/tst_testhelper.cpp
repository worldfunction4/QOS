#include <QTest>
#include "src/helper/filehelper.h"

// add necessary includes here

class TestHelper : public QObject
{
    Q_OBJECT

public:
    TestHelper();
    ~TestHelper() override;

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_readAllText();
    void test_jionPath();
};

TestHelper::TestHelper() {}

TestHelper::~TestHelper() = default;

void TestHelper::initTestCase()
{
    // code to be executed before the first test function
}

void TestHelper::init()
{
    // code to be executed before each test function
}

void TestHelper::cleanupTestCase()
{
    // code to be executed after the last test function
}

void TestHelper::cleanup()
{
    // code to be executed after each test function
}

void TestHelper::test_readAllText()
{
    QCOMPARE(FileHelper::readAllText(":/test.txt"), "这是泪叶丝用来进行测试部分的文件>w<");
}

void TestHelper::test_jionPath()
{
    QCOMPARE(FileHelper::joinPath("c:/a/b", "c"), "c:/a/b/c");
}

QTEST_APPLESS_MAIN(TestHelper)

#include "tst_testhelper.moc"
