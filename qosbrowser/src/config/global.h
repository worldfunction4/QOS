#ifndef GLOBAL_H
#define GLOBAL_H

#include "helper/filehelper.h"
#include <QString>
#include <QDir>

namespace GLOBAL
{
    namespace PATH
    {
        inline const QString WORK = QDir::currentPath();
        inline const QString TEMP = FileHelper::joinPath(WORK, "temp");
    }
    namespace SQLITE
    {
        inline const QString NAME = FileHelper::joinPath(PATH::TEMP, "cos.db");
    }
    namespace SQL
    {
        inline const QString LOGIN_INFO_PATH = ":/resource/static/sql/login_info.sql";
    }
    namespace TABLE
    {
        inline const QString LOGIN_INFO_NAME = "login_info";
    }
    namespace VERSION
    {
        inline const QString MAJOR_CUSTOM = "custom";
        inline const QString MAJOR_BUSINESS = "business";
        inline const QString JSON_PATH = ":/resource/static/versions/config_default.json";
    }
    inline bool init()
    {
        return FileHelper::makePath(PATH::TEMP);
    }
    inline bool OK = init();
}
#endif // GLOBAL_H
