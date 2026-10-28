#include "objectstablewidget.h"
#include "ui_objectstablewidget.h"
#include "src/middle/manglobal.h"
#include "src/middle/signals/mansignals.h"

ObjectsTableWidget::ObjectsTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsTableWidget)
{
    ui->setupUi(this);

    //退出登录时清空面包屑和翻页
    connect(MG->mSignal, &ManSignals::unLogin, this, [this]() {
        ui->widgetBread->clear();
        ui->widgetPage->setTotalRow(0);
    });
}

ObjectsTableWidget::~ObjectsTableWidget()
{
    delete ui;
}
