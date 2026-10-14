#include "objectwidget.h"
#include "fend/uidelegate/bucketdelegate.h"
#include "ui_objectwidget.h"
#include "src/man/manbucket.h"

ObjectWidget::ObjectWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(ManBucket::instance()->model());
    //ui->tableView->setItemDelegate(new BucketDelegate());
    ui->tableView->setItemDelegateForColumn(1, new BucketDelegate());

    //标题内容
    QStandardItemModel* model = ManBucket::instance()->model();
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

    //设置鼠标点击排序
    ui->tableView->setShortcutEnabled(true);
}

ObjectWidget::~ObjectWidget()
{
    delete ui;
}
