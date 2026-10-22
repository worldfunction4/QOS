#include "basiclogger.h"
#include "src/helper/filehelper.h"
#include "src/config/global.h"

#include <QDate>

BasicLogger::BasicLogger(QObject* parent)
    :QObject(nullptr)
{
    thread_ = new QThread();
    this->moveToThread(thread_);
    thread_->start();
}

BasicLogger::~BasicLogger()
{
    if(thread_)
    {
        thread_->quit();
        thread_->wait();
        delete thread_;
    }
}

QString BasicLogger::filePath()
{
    //返回日志文件的路径
    return FileHelper::joinPath(GLOBAL::PATH::LOG_DIR, QDate::currentDate().toString(Qt::ISODate) + ".log");
}

void BasicLogger::onLog(const QString &file, int line, const QString &func, int level, const QString &text)
{
    print(file, line, func, level, text);
}
