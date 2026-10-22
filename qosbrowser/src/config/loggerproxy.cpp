#include "loggerproxy.h"

#include <bend/dao/logs/basiclogger.h>
#include <config/global.h>

Q_GLOBAL_STATIC(LoggerProxy, ins);

LoggerProxy::LoggerProxy(QObject *parent)
    : QObject{parent}
{}

LoggerProxy::~LoggerProxy()
{
    delete logger_;
}

LoggerProxy *LoggerProxy::instance()
{
    return ins();
}

void LoggerProxy::setLogger(BasicLogger *logger)
{
    //释放旧logger并保存logger
    if(logger_)
    {
        delete logger_;
    }
    logger_ = logger;

    //进行信号槽的连接
    connect(this, &LoggerProxy::signalLog, logger_, &BasicLogger::onLog);
}

void LoggerProxy::setLevel(int level)
{
    level_ = level;
}


const LoggerProxy &LoggerProxy::reset(const QString &file, int line, const QString &func)
{
    file_ = file;
    line_ = line;
    func_ = func;
    return *this;
}

void LoggerProxy::debug(const QString &text) const
{
    doLog(text, GLOBAL::LOG_LEVEL::DEBUG);
}

void LoggerProxy::info(const QString &text) const
{
    doLog(text, GLOBAL::LOG_LEVEL::INFO);
}

void LoggerProxy::warning(const QString &text) const
{
    doLog(text, GLOBAL::LOG_LEVEL::WARNING);
}

void LoggerProxy::error(const QString &text) const
{
    doLog(text, GLOBAL::LOG_LEVEL::ERROR);
}

void LoggerProxy::fatal(const QString &text) const
{
    doLog(text, GLOBAL::LOG_LEVEL::FATAL);
}


void LoggerProxy::doLog(const QString &text, int level) const
{
    //五种等级的日志需要调用这个函数
    //对每一条日志进行检查
    //得到的是具体内容和等级
    //不返回任何东西，只发出一个信号
    if(level < level_)return;
    emit signalLog(file_, line_, func_, level, text);
}
