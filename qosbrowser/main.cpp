#include "middle/manglobal.h"
#include "src/fend/uimain/uimain.h"
#include <fend/uilogin/loginwidget.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MG->init(argc, argv);

    UiMain w;
    w.showLoginWidget();

    return QApplication::exec();
}
