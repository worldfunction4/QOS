#include "bend/dao/daobuckets.h"
#include "src/man/manbucket.h"
#include "src/fend/uimain/uimain.h"
#include <fend/uilogin/loginwidget.h>
#include "src/helper/filehelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //读取qss
    QString qss_str = FileHelper::readAllText(":/resource/static/qss/default.qss");
    a.setStyleSheet(qss_str);

    ManBucket::instance()->setManBucket();

    UiMain w;
    w.showLoginWidget();

    return QApplication::exec();
}
