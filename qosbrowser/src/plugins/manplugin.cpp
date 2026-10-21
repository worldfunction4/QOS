#include "manplugin.h"
#include "bend/dao/clouds/daocloudsmock.h"
#include "bend/dao/config/versionjson.h"
#include "src/config/global.h"
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
    delete version_;
}

ManPlugin *ManPlugin::instance()
{
    return ins();
}

void ManPlugin::installPlugins()
{
    version_ = new VersionJson(GLOBAL::VERSION::JSON_PATH);
    version_->setVersion();
    if(version_->major() == GLOBAL::VERSION::MAJOR_BUSINESS)
    {
        clouds_ = new DaoCloudsMock(":/resource/static/tests/business.json");
    }
    else
    {
        clouds_ = new DaoCloudsMock(":/resource/static/tests/custom.json");
    }
}

DaoClouds *ManPlugin::clouds() const
{
    return clouds_;
}