#include "mansignals.h"
Q_GLOBAL_STATIC(ManSignals, ins);

ManSignals::ManSignals(QObject *parent)
    : QObject{parent}
{}

ManSignals *ManSignals::instance()
{
    //第一次调用创建，之后返回同一个
    return ins();
}
