#include "manplugin.h"
#include "bend/dao/clouds/daocloudsmock.h"
#include "bend/dao/config/versioncmd.h"
#include "bend/dao/config/versionjson.h"
#include "src/config/global.h"
#include "src/config/loggerproxy.h"
#include "src/bend/dao/logs/loggerdebug.h"

Q_GLOBAL_STATIC(ManPlugin, ins);

ManPlugin::ManPlugin(QObject *parent)
    : QObject{parent}
{}

ManPlugin::~ManPlugin()
{
    delete clouds_;
    delete version_;
}

ManPlugin *ManPlugin::instance()
{
    return ins();
}

void ManPlugin::installPlugins(int argc, char *argv[])
{
    //添加日志系统
    LoggerProxy::instance()->setLogger(new LoggerDebug);
    MINFO("日志系统安装完毕");

    //增加判断：命令行参数如果有效就使用，反之就使用配置文件
    //配置选择
    if(VersionCmd(argc, argv).isValid())
    {
        version_ = new VersionCmd(argc, argv);
    }
    else
    {
        version_ = new VersionJson(GLOBAL::VERSION::JSON_PATH);
    }

    //安装云存储插件
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