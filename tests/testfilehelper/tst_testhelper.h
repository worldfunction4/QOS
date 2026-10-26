#ifndef TST_TESTHELPER_H
#define TST_TESTHELPER_H

#include <QTest>

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

#endif // TST_TESTHELPER_H
