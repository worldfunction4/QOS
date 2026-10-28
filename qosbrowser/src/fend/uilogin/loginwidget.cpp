#include "loginwidget.h"
#include "bend/gateway.h"
#include "config/apis.h"
#include "src/bend/man/mandb.h"
#include "middle/signals/mansignals.h"
#include "middle/manglobal.h"
#include "ui_loginwidget.h"

#include <QFile>
#include <QCompleter>
#include <QAbstractItemView>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>

LoginWidget::LoginWidget(QWidget *parent)
    : QosDialog(parent)
    , ui_login(new Ui::LoginWidget)
{
    ui_login->setupUi(body());
    setTitle("登录");

    ui_login->loginButton->setDefault(true);
    connect(ui_login->loginButton, &QPushButton::clicked,
            this, &LoginWidget::onLoginButtonClicked);
    connect(MG->mSignal, &ManSignals::loginSuccess, this, &LoginWidget::onLoginSuccess);
    connect(MG->mSignal, &ManSignals::error, this, &LoginWidget::onLoginError);

    resize(400, 450);
    //设置样式
    ui_login->loginButton->setProperty("style", "h5");
    ui_login->keyLabel->setProperty("style", "h5");
    ui_login->idLabel->setProperty("style", "h5");
    ui_login->markLabel->setProperty("style", "h5");
    ui_login->loginNameLabel->setProperty("style", "h5");

    //添加登录信息的下拉框
    QCompleter* completer = new QCompleter(MG->mDb->loginInfoNames(), this);

    //选择不区分大小写
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    //设置包含匹配
    completer->setFilterMode(Qt::MatchContains);

    //避免全局样式把高度压为0
    completer->popup()->setStyleSheet(
        "QAbstractItemView {"
        "  background: #ffffff;"
        "  border: 1px solid #00bcd4;"
        "  min-height: 30px;"
        "  outline: 0;"
        "}"
        );

    ui_login->loginNameLineEdit->setCompleter(completer);

    //失败槽不是登录接口就 return；是的话就弹出警告框，显示错误原因。
    connect(MG->mSignal, &ManSignals::error, this, [=](int api, const QString& msg){
        if(api != API::LOGIN::NORMAL)return;
        QMessageBox::warning(this, "错误", msg);
    });
}

LoginWidget::~LoginWidget()
{
    delete ui_login;
}

void LoginWidget::onLoginButtonClicked()
{

    //获取用户输入的账号密码，用QJsonValue临时保存
    QString secret_id = ui_login->idLineEdit->text().trimmed();
    QString secret_key = ui_login->ketLineEdit->text().trimmed();

    QJsonObject secret_obj;
    secret_obj["secretId"] = secret_id;
    secret_obj["secretKey"] = secret_key;

    //使用send发送
    MG->mGate->send(API::LOGIN::NORMAL, secret_obj);

}

void LoginWidget::onLoginSuccess()
{
    if (ui_login->checkBox->isChecked())
    {
        //勾选了记住会话：保存登录信息
        MG->mDb->saveLoginInfo(
            ui_login->loginNameLineEdit->text(),
            ui_login->idLineEdit->text(),
            ui_login->ketLineEdit->text(),
            ui_login->markLineEdit->text());
    }
    else
    {
        //没勾选：删除登录信息，只登录这一次
        MG->mDb->removeLoginInfo(ui_login->idLineEdit->text());
    }
    //关闭对话框并返回Accepted
    accept();
}

void LoginWidget::onLoginError(int api, const QString &msg)
{

    if (api != API::LOGIN::NORMAL) return;   //不是登录接口的错误就不管
    QMessageBox::warning(this, "登录失败", msg);
}
