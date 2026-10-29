#ifndef BYTEHELPER_H
#define BYTEHELPER_H

#include <QString>

class ByteHelper
{
public:
    ByteHelper();
    // 把字节数转成好读的字符串，保留两位小数，例如 1536 -> "1.53KB"
    static QString toBeautifulStr(uint64_t bytes);
};

#endif // BYTEHELPER_H
