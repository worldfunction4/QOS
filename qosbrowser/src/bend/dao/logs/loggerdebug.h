#ifndef LOGGERDEBUG_H
#define LOGGERDEBUG_H

#include "basiclogger.h"

class LoggerDebug : public BasicLogger
{
public:
    explicit LoggerDebug(QObject *parent = nullptr);
    //处理具体的函数
protected:
    void print(const QString& file, int line, const QString& func, int level, const QString& text)override;
};

#endif // LOGGERDEBUG_H
