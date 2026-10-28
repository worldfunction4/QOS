#include "buckettablewidget.h"
#include "fend/uidelegate/bucketdelegate.h"
#include "middle/manglobal.h"
#include "ui_buckettablewidget.h"
#include "src/bend/man/mancloud.h"
#include "src/middle/manglobal.h"

#include <QStandardItemModel>

BucketTableWidget::BucketTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketTableWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(MG->mCloud->model());
    //ui->tableView->setItemDelegate(new BucketDelegate());
    ui->tableView->setItemDelegateForColumn(1, new BucketDelegate());

    //标题内容
    QStandardItemModel* model = MG->mCloud->model();
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
}

BucketTableWidget::~BucketTableWidget()
{
    delete ui;
}
