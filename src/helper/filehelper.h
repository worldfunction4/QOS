#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>

class FileHelper
{
public:
    FileHelper();
    static QString readAllText(const QString& file_path);
};

#endif // FILEHELPER_H
