#include "tst_testexceptions.h"
#include "src/config/exceptions.h"

TestExceptions::TestExceptions() {}

TestExceptions::~TestExceptions() = default;

void TestExceptions::test_case1()
{
    BaseException::generateErrorCodeFile(
        QOS_DIR "/resource/static/docs/errorcode.csv",
        QOS_DIR "/src/config/errorcode.h");
}
