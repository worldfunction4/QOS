#include "buckettablewidget.h"
#include "fend/uidelegate/bucketdelegate.h"
#include "middle/manglobal.h"
#include "ui_buckettablewidget.h"
#include "src/bend/man/mancloud.h"
#include "src/middle/manglobal.h"
#include "src/middle/signals/mansignals.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"

#include <QStandardItemModel>
#include <QJsonObject>

BucketTableWidget::BucketTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketTableWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(MG->mCloud->modelBuckets());
    //ui->tableView->setItemDelegate(new BucketDelegate());
    ui->tableView->setItemDelegateForColumn(1, new BucketDelegate());

    //标题内容
    QStandardItemModel* model = MG->mCloud->modelBuckets();
    QStringList labels;
    labels << QString("桶名称")
           << QString("地区")
           << QString("创建时间");
    model->setColumnCount(labels.size());
    model->setHorizontalHeaderLabels(labels);

    //列的宽度
    for(int i=0; i < labels.count();++i)
    {
        ui->tableView->setColumnWidth(i, 200);
        if(i == labels.count()-1)
        {
            ui->tableView->horizontalHeader()->setStretchLastSection(true);//设置最后一列占满整个表
        }
    }
    //隐藏垂直标题
    ui->tableView->verticalHeader()->setHidden(true);

    //点表头可以排序
    ui->tableView->setSortingEnabled(true);

    //每页可选
    ui->widgetPage->setMaxRowList({10, 50, 100});

    //隐藏不在这一页的行
    connect(ui->widgetPage, &PageWidget::pageNumChanged,
            this, &BucketTableWidget::onPageNumChanged);

    //连接双击信号
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &BucketTableWidget::onTableViewDoubleClicked);

    //桶列表到了，更新页数
    connect(MG->mSignal, &ManSignals::bucketsSuccess, this, [this]{
        ui->widgetPage->setTotalRow(MG->mCloud->modelBuckets()->rowCount());
    });

    //退出登录时翻页回到初始状态
    connect(MG->mSignal, &ManSignals::unLogin, this, [this]{
        ui->widgetPage->setTotalRow(0);
    });
}

BucketTableWidget::~BucketTableWidget()
{
    delete ui;
}

void BucketTableWidget::onTableViewDoubleClicked(const QModelIndex &index)
{
    //桶本身的信息->桶名，地域
    QString bucketName = index.siblingAtColumn(0).data().toString();
    QString location = index.siblingAtColumn(1).data().toString();

    //打包参数，交给网关
    QJsonObject object;
    object["bucketName"] = bucketName;
    object["location"] = location;
    MG->mGate->send(API::OBJECT::LIST, object);
}

void BucketTableWidget::onPageNumChanged(int start, int len)
{
    QStandardItemModel* model = MG->mCloud->modelBuckets();
    // 一行一行看：在 [start, start+len) 范围外的就藏起来
    for (int i = 0; i < model->rowCount(); ++i)
    {
        bool hidden = (i < start || i >= start + len);
        ui->tableView->setRowHidden(i, hidden);
    }
}
