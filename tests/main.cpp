#include "testcos/tst_testcos.h"
#include "testfilehelper/tst_testhelper.h"
#include "testexceptions/tst_testexceptions.h"

int main(int argc, char *argv[])
{
    int status = 0;
    {
        TestHelper tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestExceptions tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestCos tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    return status;   // 有任何一个失败，结果就不是0
}
