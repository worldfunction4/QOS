#ifndef UIMAIN_H
#define UIMAIN_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class UiMain;
}

class LoginWidget;

class UiMain : public QWidget
{
    Q_OBJECT

public:
    explicit UiMain(QWidget *parent = nullptr);
    ~UiMain();

//显示登录窗口
    void showLoginWidget();

private:
    Ui::UiMain *ui;
    //声明一个窗口
    LoginWidget* login_ = nullptr;
};

#endif // UIMAIN_H
