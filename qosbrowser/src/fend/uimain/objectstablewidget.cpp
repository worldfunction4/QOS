#include "objectstablewidget.h"
#include "bend/man/mancloud.h"
#include "ui_objectstablewidget.h"
#include "src/middle/manglobal.h"
#include "src/middle/signals/mansignals.h"
#include "src/middle/manglobal.h"

ObjectsTableWidget::ObjectsTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsTableWidget)
{
    ui->setupUi(this);

    //显示对象 model 数据
    QStandardItemModel* model = MG->mCloud->modelObjects();
    ui->tableView->setModel(model);

    //设置表头大小时间等信息
    model->setHorizontalHeaderLabels({"名称", "大小", "修改时间"});

    //隐藏左侧的行号
    ui->tableView->verticalHeader()->setHidden(true);

    //和桶一样最后一列拉伸拉满
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

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
