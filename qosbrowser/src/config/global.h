#ifndef GLOBAL_H
#define GLOBAL_H

#include "helper/filehelper.h"
#include <QCoreApplication>
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

        //exe 旁边的 config/regions_cos.csv。窗口创建之后才能调用
        inline QString regionsCosFile()
        {
            return FileHelper::joinPath(QCoreApplication::applicationDirPath(), "config/regions_cos.csv");
        }
    }

    namespace IMG
    {
        // 主题图标所在的文件夹，换主题只改这一行
        inline const QString THEME = ":/resource/static/imgs/imgs-cyan/";
        inline const QString LOGO        = THEME + "logo.png";
        // 对话框左上角的 logo
        inline const QString QTLOGO      = THEME + "qtlogo.png";

        inline const QString BUCKET      = THEME + "bucket.png";

        inline const QString DIR         = ":/resource/static/imgs/dir.png";
        inline const QString FILE        = THEME + "file.png";

        inline const QString SEARCH      = ":/resource/static/imgs/Search.png";

        inline const QString CLOSE       = ":/resource/static/imgs/Close-1.png";
        inline const QString CLOSE_HOVER = ":/resource/static/imgs/Close-2.png";

        inline const QString MIN         = ":/resource/static/imgs/Minimize-1.png";
        inline const QString MIN_HOVER   = ":/resource/static/imgs/Minimize-2.png";

        inline const QString MAX         = ":/resource/static/imgs/Maximize-1.png";
        inline const QString MAX_HOVER   = ":/resource/static/imgs/Maximize-2.png";

        inline const QString NORMAL      = ":/resource/static/imgs/Maximize-3.png";
        inline const QString NORMAL_HOVER= ":/resource/static/imgs/Maximize-4.png";

        inline const QString RIGHT       = ":/resource/static/imgs/right.png";

        inline const QString TRANS       = THEME + "Trans-1.png";
        inline const QString TRANS_HOVER = THEME + "Trans-2.png";

        inline const QString QUIT        = THEME + "Quit-1.png";
        inline const QString QUIT_HOVER  = THEME + "Quit-2.png";

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
