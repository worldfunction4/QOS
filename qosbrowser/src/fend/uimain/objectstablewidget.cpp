#include "objectstablewidget.h"
#include "ui_objectstablewidget.h"

ObjectsTableWidget::ObjectsTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsTableWidget)
{
    ui->setupUi(this);
}

ObjectsTableWidget::~ObjectsTableWidget()
{
    delete ui;
}
