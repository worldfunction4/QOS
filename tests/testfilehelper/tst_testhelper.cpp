#include "tst_testhelper.h"
#include "src/helper/filehelper.h"

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
