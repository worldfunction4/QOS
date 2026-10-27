// qosdialog.h
#ifndef QOSDIALOG_H
#define QOSDIALOG_H

#include "basedialog.h"

namespace Ui { class QosDialog; }

// 本产品的弹窗,固定QOS的logo和标题，具体窗口都继承它
class QosDialog : public BaseDialog
{
    Q_OBJECT
public:
    explicit QosDialog(QWidget *parent = nullptr);
    ~QosDialog();

private:
    Ui::QosDialog *ui_qos;   //不能也叫ui，否则会把父类那个ui遮住
};

#endif // QOSDIALOG_H