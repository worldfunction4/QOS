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
        return data;
    }
    throw "读取失败";
}

QVariant FileHelper::readAllJson(const QString &file_path)
{
    QString data = readAllText(file_path);
    //转化为json数据
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    return doc.toVariant();
}
