#include "downloadwidget.h"
#include "middle/manglobal.h"
#include "ui_downloadwidget.h"
#include "fend/uidelegate/tableitemdelegate.h"

#include <middle/signals/mansignals.h>
#include <config/apis.h>
#include <fend/uicom/progresswidget.h>

DownloadWidget::DownloadWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    initHead();
    ui->tableWidget->setItemDelegate(new TableItemDelegate(ui->tableWidget));

    //提示文字挂在表格的可视区域上
    labelEmpty_ = new QLabel("目前没有下载任务^^", ui->tableWidget->viewport());
    //文字居中
    labelEmpty_->setAlignment(Qt::AlignCenter);
    //用 qss 里的灰色提示样式
    labelEmpty_->setProperty("style_font", "tip");
    //鼠标点击穿透到表格
    labelEmpty_->setAttribute(Qt::WA_TransparentForMouseEvents);
    //让本窗口接收可视区域的事件
    ui->tableWidget->viewport()->installEventFilter(this);
    //插入行时检查一次
    connect(ui->tableWidget->model(), &QAbstractItemModel::rowsInserted, this, &DownloadWidget::updateEmptyTip);
    //删除行时检查一次
    connect(ui->tableWidget->model(), &QAbstractItemModel::rowsRemoved, this, &DownloadWidget::updateEmptyTip);
    //启动时先算一次
    updateEmptyTip();

    //开始->加入一行
    connect(MG->mSignal, &ManSignals::startDownload, this, &DownloadWidget::onStartDownload);

    //更新进度
    connect(MG->mSignal, &ManSignals::downloadProcess, this, &DownloadWidget::onDownloadProcess);

    //成功标记完成
    connect(MG->mSignal, &ManSignals::downloadSuccess, this, &DownloadWidget::onDownloadSuccess);

    //失败标记出错
    connect(MG->mSignal, &ManSignals::error, this, &DownloadWidget::onDownloadError);
}

DownloadWidget::~DownloadWidget()
{
    delete ui;
}

bool DownloadWidget::eventFilter(QObject *obj, QEvent *event)
{
    //可视区域大小变了，提示跟着铺满
    if(obj == ui->tableWidget->viewport() && event->type() == QEvent::Resize)
        updateEmptyTip();
    //事件继续交给表格自己处理
    return QWidget::eventFilter(obj, event);
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

void DownloadWidget::onDownloadError(int api, const QString &msg, const QJsonValue &req)
{
    //不是上传的错误就不管
    if(api != API::OBJECT::GET) return;
    //从请求里取出这次任务的workId
    ProgressWidget* bar = getProgress(req["workId"].toString());
    if(!bar) return;
    //进度条显示失败
    bar->setError("下载失败");
    //鼠标悬停显示具体原因
    bar->setToolTip(msg);
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

void DownloadWidget::updateEmptyTip()
{
    //提示铺满表格的可视区域
    labelEmpty_->resize(ui->tableWidget->viewport()->size());
    //没有行才显示提示
    labelEmpty_->setVisible(ui->tableWidget->rowCount() == 0);
}
