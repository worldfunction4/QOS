#include "manglobal.h"

#include <plugins/manplugin.h>

#include <middle/signals/mansignals.h>

#include <bend/man/mancloud.h>
#include <bend/man/mandb.h>

#include <config/loggerproxy.h>

#include <bend/gateway.h>

#include <helper/filehelper.h>

#include <QApplication>

Q_GLOBAL_STATIC(ManGlobal, ins);


ManGlobal::ManGlobal(QObject *parent)
    :QObject{parent}
{
    //通过顺序来进行new
    mLog = new LoggerProxy(this);
    mPlugin = new ManPlugin(this);
    mCloud = new ManCloud(this);
    mDb = new ManDB(this);
    mSignal = new ManSignals(this);
    //由于Gateway不需要发射信号也不接收信号，不需要继承QObject，所以无法直接挂载到父对象自动析构
    mGate = new Gateway;

}

ManGlobal::~ManGlobal()
{
    delete mGate;
}

ManGlobal *ManGlobal::instance()
{
    //返回容器内的唯一指针
    return ins();
}

void ManGlobal::init(int argc, char *argv[])
{
    //读取qss
    QString qss_str = FileHelper::readAllText(":/resource/static/qss/default.qss");
    //加载qss内容
    qApp->setStyleSheet(qss_str);
    //启动插件
    MG->mPlugin->installPlugins(argc, argv);
    //模块都创建
    connect(MG->mSignal, &ManSignals::bucketsSuccess, mCloud, &ManCloud::setManBucket);
    //连接数据库
    MG->mDb->init();
    //退出登录时清理后端数据
    connect(MG->mSignal, &ManSignals::unLogin, mCloud, &ManCloud::onUnLogin);
}
