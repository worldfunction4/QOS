#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include "src/fend/uicom/qosdialog.h"
#include <QWidget>

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QosDialog
{
    Q_OBJECT

public:
    explicit TransferDialog(QWidget *parent = nullptr);
    ~TransferDialog();

    //切换到上传页和下载页
    void showPage(int index);

private:
    Ui::TransferDialog *ui_transfer;
};

#endif // TRANSFERDIALOG_H
