#ifndef VERSIONJSON_H
#define VERSIONJSON_H

#include "version.h"

//使用json配置文件读取版本信息
class VersionJson : public Version
{
public:
    VersionJson(const QString& path);

    void setVersion()override;

private:
    const QString path_; //保存配置文件路径
};

#endif // VERSIONJSON_H
