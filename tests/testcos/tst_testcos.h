#ifndef TST_TESTCOS_H
#define TST_TESTCOS_H

#include <QTest>
#include "bend/dao/clouds/daocloudscos.h"

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
    void test_putBucket();
    void test_putBucketAgain();
    void test_deleteBucket();
private:
    DaoCloudsCos cos_;
    QString secretId_;
    QString secretKey_;
};

#endif // TST_TESTCOS_H
