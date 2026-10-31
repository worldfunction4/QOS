#include "uimain.h"
#include "ui_uimain.h"
#include "src/fend/uilogin/loginwidget.h"
#include "src/fend/uitransfer/transferdialog.h"
#include "src/middle/signals/mansignals.h"
#include "src/middle/manglobal.h"
#include "src/config/global.h"

#include <QWidget>
#include <QPushButton>

#include <QDebug>

UiMain::UiMain(QWidget *parent)
    : QosDialog(parent)
    , ui_main(new Ui::UiMain)
{
    //界面放入父类内容区域
    ui_main->setupUi(body());

    //拖拽时只进行预览不持续实时调整布局,鼠标松开后真正调整
    ui_main->splitter->setOpaqueResize(false);

    ui_main->splitter->setStretchFactor(0, 1);
    ui_main->splitter->setStretchFactor(1, 4);

    //设置默认宽与高1000*700
    this->resize(1000, 700);

    //先创建传输列表
    transfer_ = new TransferDialog(this);

    //开始上传或下载时自动跳转
    connect(MG->mSignal, &ManSignals::startUpload, transfer_, [this]{
        transfer_->showPage(0);
    });
    connect(MG->mSignal, &ManSignals::startDownload, transfer_, [this]{
        transfer_->showPage(1);
    });

    //传输列表按钮
    QPushButton* transButton = addButton(GLOBAL::IMG::TRANS,
                                         GLOBAL::IMG::TRANS_HOVER);

    connect(transButton, &QPushButton::clicked, this, &UiMain::onTransferClicked);

    //退出登录按钮
    QPushButton* quitButton = addButton(GLOBAL::IMG::QUIT,
                                        GLOBAL::IMG::QUIT_HOVER);

    connect(quitButton, &QPushButton::clicked, this, &UiMain::onQuitClicked);

    //对象列表数据到了，切换到对象页
    connect(MG->mSignal, &ManSignals::objectsSuccess, this, [this]{
        ui_main->stackedWidget->setCurrentIndex(1);
    });

    //分割线
    addTitleLine();

    //最小化
    addMinButton(GLOBAL::IMG::MIN, GLOBAL::IMG::MIN_HOVER);
    addMaxButton(GLOBAL::IMG::MAX, GLOBAL::IMG::MAX_HOVER,
                 GLOBAL::IMG::NORMAL, GLOBAL::IMG::NORMAL_HOVER);

    //esc或回车不能退出窗口
    setKeyDisabled();

    //添加logo图标
    this->setWindowIcon(QIcon(":/resource/static/imgs/logo.png"));

    //默认显示桶列表
    ui_main->stackedWidget->setCurrentIndex(0);

    //桶列表数据到了，切换到桶页
    connect(MG->mSignal, &ManSignals::bucketsSuccess, this, [this]{
        ui_main->stackedWidget->setCurrentIndex(0);
    });

    //登录成功->主界面
    connect(MG->mSignal, &ManSignals::loginSuccess, this, &UiMain::show);

    //退出登录时主界面复位
    connect(MG->mSignal, &ManSignals::unLogin, this, &UiMain::onUnLogin);
}

UiMain::~UiMain()
{
    delete ui_main;
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
    }
    login_ -> show();
}

void UiMain::onTransferClicked()
{
    //显示
    transfer_->show();
}

void UiMain::onQuitClicked()
{
    emit MG->mSignal->unLogin();
}

void UiMain::onUnLogin()
{
    //回到桶列表页
    ui_main->stackedWidget->setCurrentIndex(0);

    //隐藏主界面
    hide();
    showLoginWidget();
}
