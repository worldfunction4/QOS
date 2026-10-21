#include "versionjson.h"

#include <helper/filehelper.h>

#include <QJsonObject>

VersionJson::VersionJson(const QString &path)
    :path_(path)
{}

void VersionJson::setVersion()
{
    //拿到配置文件内容
    QJsonObject obj = FileHelper::readAllJson(path_).toJsonObject();
    //读取版本内容
    obj = obj["version"].toObject();
    m_major = obj["major"].toString();
    m_dev = obj["dev"].toString();
    m_v1 = obj["v1"].toInt();
    m_v2 = obj["v2"].toInt();
    m_v3 = obj["v3"].toInt();
}