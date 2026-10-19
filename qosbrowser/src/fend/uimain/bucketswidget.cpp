#include "bucketswidget.h"
#include "ui_bucketswidget.h"
#include "src/bend/man/manbucket.h"

BucketsWidget::BucketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketsWidget)
{
    ui->setupUi(this);
    ui->listView->setModel(ManBucket::instance()->model());

}

BucketsWidget::~BucketsWidget()
{
    delete ui;
}