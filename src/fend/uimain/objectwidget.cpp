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
}

ObjectWidget::~ObjectWidget()
{
    delete ui;
}
