#include <QTest>
#include "src/config/exceptions.h"

// add necessary includes here

class TestExceptions : public QObject
{
    Q_OBJECT

public:
    TestExceptions();
    ~TestExceptions() override;

private slots:
    void test_case1();
};

TestExceptions::TestExceptions() {}

TestExceptions::~TestExceptions() = default;

void TestExceptions::test_case1()
{
    BaseException::generateErrorCodeFile(
        QOS_DIR "/resource/static/docs/errorcode.csv",
        QOS_DIR "/src/config/errorcode.h");
}

QTEST_APPLESS_MAIN(TestExceptions)

#include "tst_testexceptions.moc"
