#include "exceptions.h"

BaseException::BaseException(const QString& code,
                             const QString& message)
    :code_(code), msg_(message)
{}

QString BaseException::msg() const
{
    return QString("错误(%1): %2").arg(code_)
                                .arg(msg_);
}

QString BaseException::code() const
{
    return code_;
}
