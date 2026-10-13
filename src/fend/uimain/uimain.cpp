#include "uimain.h"
#include "ui_uimain.h"
#include "src/fend/uilogin/loginwidget.h"

UiMain::UiMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiMain)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);
    //设置默认宽与高1000*700
    this->resize(1000, 700);

    //添加logo图标
    this->setWindowIcon(QIcon("D:/QT/projects/Q/MainWindow/resource/static/imgs/logo.png"));
    connect(ui->widgetToolBar, &ToolBarWidget::loginout, this, [=]{
        this->hide();
        showLoginWidget();
    });
}

UiMain::~UiMain()
{
    delete ui;
    if(login_)
    {
        delete login_;
    }
}

void UiMain::showLoginWidget()
{
    if(login_ == nullptr)
    {
        login_ = new LoginWidget;
        //显示主窗口
        connect(login_, &LoginWidget::loginSucessfuly, this, &UiMain::show);
    }
    login_ -> show();
}
