#include "versioncmd.h"
#include "src/config/global.h"
#include <QList>

VersionCmd::VersionCmd(int argc, char *argv[])
{
    if(argc == 2)
    {
        QString args = argv[1];
        //使用.进行切割,比如business.dev.1.0.2->[business,dev,1,0,2]
        //考虑到大小写的情况，使用toLower转小写
        argv_ = args.toLower().split(".");
    }
}

void VersionCmd::setVersion()
{
    //有五个参数
    if(!isValid())
    {
        throw QString("格式错误%1").arg(argv_.join("."));
    }
    if(argv_[0] != GLOBAL::VERSION::MAJOR_BUSINESS && argv_[0] != GLOBAL::VERSION::MAJOR_CUSTOM)
    {
        throw QString("主版本是是未知的：%1").arg(argv_[0]);
    }
    m_major = argv_[0];
    m_env = argv_[1];
    m_v1 = argv_[2].toInt();
    m_v2 = argv_[3].toInt();
    m_v3 = argv_[4].toInt();
}

bool VersionCmd::isValid()const
{
    return argv_.size() == 5;
}
