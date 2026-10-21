#include "plugins/manplugin.h"
#include "src/bend/man/mandb.h"
#include "src/bend/man/mancloud.h"
#include "src/fend/uimain/uimain.h"
#include <fend/uilogin/loginwidget.h>
#include "src/helper/filehelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //读取qss
    QString qss_str = FileHelper::readAllText(":/resource/static/qss/default.qss");
    //加载qss内容
    a.setStyleSheet(qss_str);
    //启动插件
    ManPlugin::instance()->installPlugins(argc, argv);
    //调用
    ManCloud::instance()->setManBucket();
    //连接数据库
    ManDB::instance()->init();

    UiMain w;
    w.showLoginWidget();

    return QApplication::exec();
}
