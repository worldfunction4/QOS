#include "downloadwidget.h"
#include "middle/manglobal.h"
#include "ui_downloadwidget.h"

#include <middle/signals/mansignals.h>

#include <fend/uicom/progresswidget.h>

DownloadWidget::DownloadWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    initHead();

    //开始->加入一行
    connect(MG->mSignal, &ManSignals::startDownload, this, &DownloadWidget::onStartDownload);

    //更新进度
    connect(MG->mSignal, &ManSignals::downloadProcess, this, &DownloadWidget::onDownloadProcess);

    //成功标记完成
    connect(MG->mSignal, &ManSignals::downloadSuccess, this, &DownloadWidget::onDownloadSuccess);
}

DownloadWidget::~DownloadWidget()
{
    delete ui;
}

void DownloadWidget::onStartDownload(const QString &workId, const QString &key, const QString &localPath, long long total)
{
    //最上面插入一行
    ui->tableWidget->insertRow(0);

    //下标0：云端路径，workId藏在这一格
    QTableWidgetItem* item = new QTableWidgetItem(key);
    item->setData(Qt::UserRole, workId);
    ui->tableWidget->setItem(0, 0, item);

    //下标1：本地保存路径
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(localPath));

    //下标2：进度条，总大小来自云端对象的size
    ProgressWidget* bar = new ProgressWidget();
    bar->setRange(0, total);
    ui->tableWidget->setCellWidget(0, 2, bar);
}

void DownloadWidget::onDownloadProcess(const QString &workId, long long transferred, long long)
{
    ProgressWidget* bar = getProgress(workId);
    if(!bar)return;
    bar->setValue(transferred);
}

void DownloadWidget::onDownloadSuccess(const QString &workId)
{
    ProgressWidget* bar = getProgress(workId);
    if(!bar)return;
    bar->setFinished("下载成功^^");
}

ProgressWidget *DownloadWidget::getProgress(const QString &workId)
{
    //遍历每一行
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        //取下标0那一格藏的workId
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if(item && item->data(Qt::UserRole).toString() == workId)
        {
            //下标2的控件转回进度条
            return qobject_cast<ProgressWidget*>(ui->tableWidget->cellWidget(i, 2));
        }
    }
    //没找到
    return nullptr;
}

void DownloadWidget::initHead()
{
    QStringList headers;
    headers << "云端路径" << "本地路径" << "下载状态";
    //列数=表头个数
    ui->tableWidget->setColumnCount(headers.size());
    //设置顶部横向表头
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //最后一列占满剩余宽度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //路径列固定宽度，太长的显示省略号
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 200);
}