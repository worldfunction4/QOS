#ifndef TST_TESTEXCEPTION_H
#define TST_TESTEXCEPTION_H

#include <QTest>

class TestExceptions : public QObject
{
    Q_OBJECT

public:
    TestExceptions();
    ~TestExceptions() override;

private slots:
    void test_case1();
};

#endif // TST_TESTEXCEPTION_H
