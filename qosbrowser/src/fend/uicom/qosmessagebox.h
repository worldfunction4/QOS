#ifndef QOSMESSAGEBOX_H
#define QOSMESSAGEBOX_H

#include "qosdialog.h"

namespace Ui {
class QosMessageBox;
}

//本产品统一样式的提示框
class QosMessageBox : public QosDialog
{
    Q_OBJECT

public:
    explicit QosMessageBox(QWidget *parent = nullptr);
    ~QosMessageBox();

    //显示提示框，返回用户点的按钮文字；默认只有一个"确定"
    QString showMessage(const QString& title,
                        const QString& text,
                        const QStringList& buttonTexts = QStringList{"确定"});

private:
    //根据文字列表创建按钮
    void createButtons(const QStringList& buttonTexts);

private:
    Ui::QosMessageBox *ui_box;
    //记录用户点了哪个按钮
    QString clickedText_;
};

#endif // QOSMESSAGEBOX_H