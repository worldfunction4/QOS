#include "uimain.h"
#include "ui_uimain.h"
#include "src/fend/uilogin/loginwidget.h"
#include "src/middle/signals/mansignals.h"

UiMain::UiMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiMain)
{
    ui->setupUi(this);

    //拖拽时只进行预览不持续实时调整布局,鼠标松开后真正调整
    ui->splitter->setOpaqueResize(false);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 4);
    //设置默认宽与高1000*700
    this->resize(1000, 700);

    //添加logo图标
    this->setWindowIcon(QIcon(":/resource/static/imgs/logo.png"));
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
        connect(ManSignals::instance(), &ManSignals::loginSuccess, this, &UiMain::show);
    }
    login_ -> show();
}
