#include "filehelper.h"

#include <QFile>

FileHelper::FileHelper() {}

QString FileHelper::readAllText(const QString &file_path)
{
    QFile file(file_path);
    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();
        return QString(data);
    }
    throw "读取失败";
}
