#include "filehelper.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
//Qt6不用QRegExp
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

QList<QStringList> FileHelper::readAllCsv(const QString &file_path)
{
    QList<QStringList> file_csv;

    //读取整个文本
    QString data = readAllText(file_path);
    //按\r?\n切成行，跳过空行
    QStringList lines = data.split(QRegularExpression("\r?\n"),Qt::SkipEmptyParts);
    //遍历每一行
    for(const QString& line : lines)
    {
        //每行按逗号切成列
        QStringList row = line.split(",");

        file_csv.append(row);
    }
    return file_csv;
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

void FileHelper::writeFile(const QStringList &lines, const QString &file_path)
{
    //以只写方式打开文件
    QFile file(file_path);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for(const QString& line : lines)
        {
            //每个元素写成一行
            stream << line << "\n";
        }
        file.close();
        return;
    }
    throw "写入文件失败";
}


