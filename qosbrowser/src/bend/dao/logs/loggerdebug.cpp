#include "loggerdebug.h"
#include "src/config/global.h"

#include <QDateTime>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

LoggerDebug::LoggerDebug(QObject *parent)
    : BasicLogger{parent}
{}

void LoggerDebug::print(const QString &file, int line, const QString &func, int level, const QString &text)
{
    //获取当前时间
    QString now_time = QDateTime::currentDateTime().toString(Qt::ISODate);
    //将相应等级以字符串形式保存
    QString level_name = GLOBAL::LOG_LEVEL_NAME[level];
    //只保留文件名，去掉前面的路径
    QString file_name = QFileInfo(file).fileName();
    //拼接
    QString msg = QString("%1 [%2] %3:%4 %5 - %6")
                        .arg(now_time)
                        .arg(level_name)
                        .arg(file_name)
                        .arg(line)
                        .arg(func)
                        .arg(text);

    //写入文件
    QFile log(filePath());

    //选择写入模式或追加模式
    if(log.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&log);
        stream << msg << "\n";;
    }
    qDebug().noquote() << msg;
}
