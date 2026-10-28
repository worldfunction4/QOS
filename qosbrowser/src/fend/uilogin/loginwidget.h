#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "fend/uicom/qosdialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QString>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QosDialog
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
private slots:
    void onLoginButtonClicked();
    void onLoginSuccess();
    void onLoginError(int api, const QString& msg);

private:
    Ui::LoginWidget *ui_login;
};

#endif // LOGINWIDGET_H
