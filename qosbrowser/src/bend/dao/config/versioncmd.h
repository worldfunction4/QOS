#ifndef VERSIONCMD_H
#define VERSIONCMD_H

#include <QStringList>

#include "version.h"

class VersionCmd : public Version
{
public:
    //使用命令行传参
    VersionCmd(int argc, char* argv[]);
    void setVersion()override;
    //检查参数是否正确
    bool isValid()const;
private:
    QStringList argv_;
};

#endif // VERSIONCMD_H
