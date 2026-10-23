#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <QString>

class BaseException
{
public:
    //错误码+具体内容
    explicit BaseException(const QString& code,
                           const QString& message);
    //返回错误码:信息
    QString msg()const;
    //返回错误码
    QString code()const;

private:
    QString code_;
    QString msg_;
};

#endif // BASEEXCEPTION_H
