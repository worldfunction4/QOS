#include "manplugin.h"
#include "bend/dao/clouds/daocloudsmock.h"

Q_GLOBAL_STATIC(ManPlugin, ins);

ManPlugin::ManPlugin(QObject *parent)
    : QObject{parent}
{
    //在初始化管理器的时候直接安装,防止clouds返回空指针导致程序崩溃
    installPlugins();
}

ManPlugin::~ManPlugin()
{
    delete clouds_;
}

ManPlugin *ManPlugin::instance()
{
    return ins();
}

void ManPlugin::installPlugins()
{
    clouds_ = new DaoCloudsMock(":/resource/static/tests/buckets2.json");
}

DaoClouds *ManPlugin::clouds() const
{
    return clouds_;
}