#include "loginwidget.h"
#include "man/mandb.h"
#include "ui_loginwidget.h"
#include <QFile>
#include <QCompleter>
#include <QAbstractItemView>

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
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginButton_clicked()
{
    //验证信息
    if(ui->idLineEdit->text().trimmed() == "admin" &&
        ui->ketLineEdit->text().trimmed() == "admin123")
    {
        emit loginSucessfuly();
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
    }
    else
    {
        QMessageBox::warning(
            this,
            "错误",
            "检查用户名或密码是否正确"
        );
        return;
    }
}
