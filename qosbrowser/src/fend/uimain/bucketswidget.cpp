#include "bucketswidget.h"
#include "ui_bucketswidget.h"
#include "src/bend/man/mancloud.h"
#include "src/middle/manglobal.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"

#include <QJsonObject>

BucketsWidget::BucketsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BucketsWidget)
{
    ui->setupUi(this);
    ui->listView->setModel(MG->mCloud->modelBuckets());

    // 双击左边的桶，也去取它的对象
    connect(ui->listView, &QListView::doubleClicked, this, [](const QModelIndex& index) {
        QJsonObject params;
        params["bucketName"] = index.siblingAtColumn(0).data().toString();
        params["location"] = index.siblingAtColumn(1).data().toString();
        MG->mGate->send(API::OBJECT::LIST, params);
    });
}

BucketsWidget::~BucketsWidget()
{
    delete ui;
}