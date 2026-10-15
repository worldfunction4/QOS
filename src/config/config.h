#ifndef CONFIG_H
#define CONFIG_H

#include "helper/filehelper.h"
#include <QString>
#include <QDir>

namespace CONFIG
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
    inline bool init()
    {
        return FileHelper::makePath(PATH::TEMP);
    }
    inline bool OK = init();
}
#endif // CONFIG_H
