#ifndef UIMAIN_H
#define UIMAIN_H

#include <QDialog>

#include <fend/uicom/qosdialog.h>

namespace Ui {
class UiMain;
}

class LoginWidget;

class UiMain : public QosDialog
{
    Q_OBJECT

public:
    explicit UiMain(QWidget *parent = nullptr);
    ~UiMain();

    //显示登录窗口
    void showLoginWidget();

private slots:
    //传输列表
    void onTransferClicked();
    //退出登录
    void onQuitClicked();
    //通知所有模块退出登录
    void onUnLogin();

private:
    Ui::UiMain *ui_main;
    //声明一个窗口
    LoginWidget* login_ = nullptr;
};

#endif // UIMAIN_H
