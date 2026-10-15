#include "filehelper.h"

#include <QDir>
#include <QFile>
//在Qt6中已经不用#include <QRegExp>
#include <QRegularExpression>

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

QString FileHelper::joinPath(const QString &path1, const QString &path2)
{
    QString path = path1 + "/" + path2;
    //处理一个连接的path
    QStringList path_list = path.split(QRegularExpression(R"([/\\])"), Qt::SkipEmptyParts);
    //重新进行拼接吧
    path = path_list.join("/");
    //处理完毕后清理，比如把project/..等价于project
    return QDir::cleanPath(path);
}

bool FileHelper::makePath(const QString &path)
{
    return QDir().mkpath(path);
}


