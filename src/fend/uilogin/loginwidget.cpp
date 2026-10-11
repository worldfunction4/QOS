#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
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
void LoginWidget::on_closeButton_clicked()
{
    QApplication::quit();//退出整个应用程序
}

