#include "uploadwidget.h"
#include "middle/manglobal.h"
#include "ui_uploadwidget.h"

#include <middle/signals/mansignals.h>

#include <fend/uicom/progresswidget.h>

UploadWidget::UploadWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadWidget)
{
    ui->setupUi(this);
    initHead();

    //开始->加入一行
    connect(MG->mSignal, &ManSignals::startUpload, this, &UploadWidget::onStartUpload);

    //更新进度
    connect(MG->mSignal, &ManSignals::uploadProcess, this, &UploadWidget::onUploadProcess);

    //成功标记完成
    connect(MG->mSignal, &ManSignals::uploadSuccess, this, &UploadWidget::onUploadSuccess);
}

UploadWidget::~UploadWidget()
{
    delete ui;
}

void UploadWidget::onStartUpload(const QString &workId, const QString &key, const QString &localPath, long long total)
{
    //最上面插入一行，新任务加在顶部
    ui->tableWidget->insertRow(0);

    //云端路径
    QTableWidgetItem* item = new QTableWidgetItem(key);

    //workid 放入这一格并隐藏
    item->setData(Qt::UserRole, workId);
    ui->tableWidget->setItem(0, 0, item);

    //第一列：本地路径
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(localPath));

    //第二列：进度条
    ProgressWidget* bar = new ProgressWidget();
    //总大小
    bar->setRange(0, total);
    //表格接管bar，无需手动析构
    ui->tableWidget->setCellWidget(0, 2, bar);
}

void UploadWidget::onUploadProcess(const QString &workId, long long transferred, long long)
{
    ProgressWidget* bar = getProgress(workId);
    if(!bar)return;
    bar->setValue(transferred);
}

void UploadWidget::onUploadSuccess(const QString &workId)
{
    ProgressWidget* bar = getProgress(workId);
    if(!bar)return;
    bar->setFinished("上传成功^^");
}

ProgressWidget *UploadWidget::getProgress(const QString &workId)
{
    //遍历行数
    for(int i=0; i < ui->tableWidget->rowCount(); ++i)
    {
        //找到workid
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if(item && item->data(Qt::UserRole).toString() == workId)
        {
            //把第二列的空间转回进度条
            return qobject_cast<ProgressWidget*>(ui->tableWidget->cellWidget(i, 2));
        }
    }
    //没找到？
    return nullptr;
}

void UploadWidget::initHead()
{
    QStringList headers;
    headers << "云端路径" << "本地路径" << "上传状态";
    //列数=表头个数
    ui->tableWidget->setColumnCount(headers.size());
    //设置为顶部横向表头
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //最后一列占满剩余的宽度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //路径列固定宽度，太长的显示省略号
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 200);
}
