#ifndef MANPLUGIN_H
#define MANPLUGIN_H

#include "bend/dao/clouds/daoclouds.h"
#include "bend/dao/config/version.h"
#include <QObject>

//设置为单例模式
class ManPlugin : public QObject
{
    Q_OBJECT
public:
    static ManPlugin* instance();
signals:

public:
    //初始化插件
    void installPlugins();
    DaoClouds* clouds()const;
    explicit ManPlugin(QObject *parent = nullptr);
    ~ManPlugin();
private:
    //保存installPlugins()中new出来的地址
    //另外这是基类指针指向派生类对象，谁构造谁析构
    DaoClouds* clouds_ = nullptr;
    //传递一个指向版本的指针
    Version* version_ = nullptr;
};

#endif // MANPLUGIN_H
