#include "loginwidget.h"
#include "bend/gateway.h"
#include "config/apis.h"
#include "src/bend/man/mandb.h"
#include "middle/signals/mansignals.h"
#include "ui_loginwidget.h"

#include <QFile>
#include <QCompleter>
#include <QAbstractItemView>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>

LoginWidget::LoginWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resource/static/imgs/logo.png"));

    //设置样式
    ui->loginButton->setProperty("style", "h5");
    ui->keyLabel->setProperty("style", "h5");
    ui->idLabel->setProperty("style", "h5");
    ui->markLabel->setProperty("style", "h5");
    ui->loginNameLabel->setProperty("style", "h5");

    //添加登录信息的下拉框
    QCompleter* completer = new QCompleter(ManDB::instance()->loginInfoNames(), this);

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

    ui->loginNameLineEdit->setCompleter(completer);
    //选择登录名时自动填充
    connect(completer, QOverload<const QString&>::of(&QCompleter::activated), this, [=](const QString& name){
        LoginInfo info;
        if(ManDB::instance()->loginInfoByName(name, &info))
        {
            ui->loginNameLineEdit->setText(info.name);
            ui->idLineEdit->setText(info.secretId);
            ui->ketLineEdit->setText(info.secretKey);
            ui->markLineEdit->setText(info.remark);
            ui->checkBox->setChecked(true);
        }
    });

    //成功槽根据复选框保存或删除登录信息，然后关闭窗口。
    connect(ManSignals::instance(), &ManSignals::loginSuccess, this, [=]{
        if(ui->checkBox->isChecked())
        {
            //那么就保存登录信息
            ManDB::instance()->saveLoginInfo(
                ui->loginNameLineEdit->text(),
                ui->idLineEdit->text(),
                ui->ketLineEdit->text(),
                ui->markLineEdit->text()
                );
        }
        else
        {
            //不然就删除登录信息(不保存，一次性登录)
            ManDB::instance()->removeLoginInfo(ui->idLineEdit->text());
        }
        this->close();
    });
    //失败槽不是登录接口就 return；是的话就弹出警告框，显示错误原因。
    connect(ManSignals::instance(), &ManSignals::error, this, [=](int api, const QString& msg){
        if(api != API::LOGIN::NORMAL)return;
        QMessageBox::warning(this, "错误", msg);
    });
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginButton_clicked()
{

    //获取用户输入的账号密码，用QJsonValue临时保存
    QString secret_id = ui->idLineEdit->text().trimmed();
    QString secret_key = ui->ketLineEdit->text().trimmed();

    QJsonObject secret_obj;
    secret_obj["secretId"] = secret_id;
    secret_obj["secretKey"] = secret_key;

    //使用send发送
    Gateway::instance()->send(API::LOGIN::NORMAL, secret_obj);

}
