#ifndef LOGGERPROXY_H
#define LOGGERPROXY_H

#include <QObject>

class BasicLogger;

class LoggerProxy : public QObject
{
    Q_OBJECT
public:
    explicit LoggerProxy(QObject *parent = nullptr);
    ~LoggerProxy();
    //使用单例
    static LoggerProxy* instance();

    //设置安装的日志系统
    void setLogger(BasicLogger* logger);
    //设置过滤的日志等级
    void setLevel(int level);

    //接收信息用成员变量进行存储
    const LoggerProxy& reset(const QString& file, int line, const QString& func);

    //给日志划分等级
    void debug(const QString& text)const;
    void info(const QString& text)const;
    void warning(const QString& text)const;
    void error(const QString& text)const;
    void fatal(const QString& text)const;

signals:
    void signalLog(const QString &file, int line, const QString &func, int level, const QString &text)const;
private:
    //几个不同等级的日志相同逻辑
    void doLog(const QString& text, int level)const;
    //保留日志的指针
    BasicLogger* logger_ = nullptr;
    //过滤等级，默认为0全部输出
    int level_ = 0;
    //记录文件
    QString file_ = "full";
    //记录函数名
    QString func_ = "full";
    //记录行数
    int line_ = 0;
};

//定义宏
#define LP LoggerProxy::instance()
#define MDEBUG LP->reset(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#define MINFO LP->reset(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).info
#define MWARNING LP->reset(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning
#define MERROR LP->reset(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).error
#define MFATAL LP->reset(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).fatal

#endif // LOGGERPROXY_H
