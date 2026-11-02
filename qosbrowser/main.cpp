#include "middle/manglobal.h"
#include "fend/uimain/uimain.h"
#include "fend/uilogin/loginwidget.h"
#include "config/global.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //程序名，没有设置标题的窗口都显示这个
    QApplication::setApplicationDisplayName("QOS");
    //所有窗口默认用这个图标
    QApplication::setWindowIcon(QIcon(GLOBAL::IMG::LOGO));

    MG->init(argc, argv);

    UiMain w;
    w.showLoginWidget();

    return QApplication::exec();
}
