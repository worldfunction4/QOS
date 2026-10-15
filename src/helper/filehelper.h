#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <QJsonDocument>
#include <QVariant>

class FileHelper
{
public:
    FileHelper() = delete;
    static QString readAllText(const QString& file_path);
    static QVariant readAllJson(const QString& file_path);
    static QString joinPath(const QString& path1, const QString& path2);
    static bool makePath(const QString& path);

};

#endif // FILEHELPER_H
