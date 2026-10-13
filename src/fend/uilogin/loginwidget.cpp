#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QFile>
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
