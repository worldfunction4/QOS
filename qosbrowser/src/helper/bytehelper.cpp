#include "bytehelper.h"

ByteHelper::ByteHelper() {}

QString ByteHelper::toBeautifulStr(uint64_t bytes)
{
    //单位表，从小到大
    static const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    //先当成小数，才能保留一位小数
    double size = bytes;
    int i = 0;
    //每满1024就进一个单位
    while (size >= 1024 && i < 4) {
        size /= 1024;
        ++i;
    }
    // B 不要小数，其余保留一位
    if (i == 0)
        return QString("%1 B").arg(bytes);
    return QString("%1 %2").arg(size, 0, 'f', 2).arg(units[i]);
}
