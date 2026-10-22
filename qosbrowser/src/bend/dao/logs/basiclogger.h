#ifndef BASICLOGGER_H
#define BASICLOGGER_H

#include <QObject>
#include <QThread>
#include <QString>

//日志模板的基类
class BasicLogger : public QObject
{
    Q_OBJECT
public:
    BasicLogger(QObject* parent = nullptr);
    virtual ~BasicLogger();
    //返回日志文件路径
    static QString filePath();
public slots:
    //接收代理信号发射的槽函数
    void onLog(const QString& file, int line, const QString& func, int level, const QString& text);
protected:
    virtual void print(const QString& file, int line, const QString& func, int level, const QString& text)=0;
private:
    QThread* thread_=nullptr;
};

#endif // BASICLOGGER_H
