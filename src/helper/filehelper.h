#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <QJsonDocument>
#include <QVariant>

class FileHelper
{
public:
    FileHelper();
    static QString readAllText(const QString& file_path);
    static QVariant readAllJson(const QString& file_path);
};

#endif // FILEHELPER_H
