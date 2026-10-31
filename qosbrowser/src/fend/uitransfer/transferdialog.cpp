#include "uploadwidget.h"
#include "downloadwidget.h"
#include "transferdialog.h"
#include "ui_transferdialog.h"

TransferDialog::TransferDialog(QWidget *parent)
    : QosDialog(parent)
    , ui_transfer(new Ui::TransferDialog)
{
    ui_transfer->setupUi(body());

    //设计删不了，用代码把自带的两个页删除了
    ui_transfer->tabWidget->clear();

    //标题
    setTitle("传输列表");

    //大小固定
    setFixedSize(800, 500);

    //上传页
    ui_transfer->tabWidget->addTab(new UploadWidget(), "上传");
    //下载页
    ui_transfer->tabWidget->addTab(new DownloadWidget(), "下载");
}

TransferDialog::~TransferDialog()
{
    delete ui_transfer;
}

void TransferDialog::showPage(int index)
{
    ui_transfer->tabWidget->setCurrentIndex(index);
    show();
    //已经打开提到最前面
    raise();
}
