#include "objectwidget.h"
#include "ui_objectwidget.h"
#include "src/man/manbucket.h"

ObjectWidget::ObjectWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(ManBucket::instance()->model());
}

ObjectWidget::~ObjectWidget()
{
    delete ui;
}
