#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <QString>
#include <QMap>

using ErrorMap = QMap<QString, QString>;

class BaseException
{
public:
    //错误码+具体内容
    explicit BaseException(const QString& code,
                           const QString& message);
    //获取错误码
    static ErrorMap parseErrorCode(const QString& csv_path);
    //自动生成错误码列表
    static void generateErrorCodeFile(const QString& csv_path, const QString& target_path);
    //返回错误码:信息
    QString msg()const;
    //返回错误码
    QString code()const;

private:
    QString code_;
    QString msg_;
};

#endif // BASEEXCEPTION_H
