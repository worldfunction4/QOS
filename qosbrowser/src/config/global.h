#ifndef GLOBAL_H
#define GLOBAL_H

#include "helper/filehelper.h"
#include <QString>
#include <QStringList>
#include <QDir>

namespace GLOBAL
{
    namespace PATH
    {
        inline const QString WORK = QDir::currentPath();
        inline const QString TEMP = FileHelper::joinPath(WORK, "temp");
        inline const QString LOG_DIR = FileHelper::joinPath(WORK, "logs");
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

    enum LOG_LEVEL
    {
        DEBUG = 0,   //调试用
        INFO = 1,    //普通日志
        WARNING = 2, //警告
        ERROR = 3,   //出错，但程序仍能运行
        FATAL = 4    //严重错误，程序无法运行
    };
    //将枚举的数字转换为字符串方便后续打印
    inline const QStringList LOG_LEVEL_NAME = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};

    inline bool init()
    {
        return FileHelper::makePath(PATH::TEMP) && FileHelper::makePath(PATH::LOG_DIR);
    }
    inline bool OK = init();
}
#endif // GLOBAL_H
