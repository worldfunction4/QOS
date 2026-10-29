#include "objectstablewidget.h"
#include "bend/man/mancloud.h"
#include "ui_objectstablewidget.h"
#include "src/middle/manglobal.h"
#include "src/middle/signals/mansignals.h"
#include "src/middle/manglobal.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"

#include <QJsonObject>

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

    //名称列宽一些，大小列适中， 最后一列自动拉伸
    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(1, 120);
    //点表头就能排序
    ui->tableView->setSortingEnabled(true);

    //隐藏左侧的行号
    ui->tableView->verticalHeader()->setHidden(true);

    //和桶一样最后一列拉伸拉满
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //退出登录时清空面包屑和翻页
    connect(MG->mSignal, &ManSignals::unLogin, this, [this]() {
        ui->widgetBread->clear();
        ui->widgetPage->setTotalRow(0);
    });

    //对象列表到了->面包屑显示
    connect(MG->mSignal, &ManSignals::objectsSuccess, this, [this]{
        QString path = MG->mCloud->currentBucket() + "/" + MG->mCloud->currentDir();
        ui->widgetBread->setPath(path);

        //告诉翻页空间一共几条
        ui->widgetPage->setTotalRow(MG->mCloud->modelObjects()->rowCount());
    });

    //双击表格
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &ObjectsTableWidget::onTableViewDoubleClicked);

    //面包屑
    connect(ui->widgetBread, &BreadWidget::pathChanged,
            this, &ObjectsTableWidget::onPathChanged);
    connect(ui->widgetBread, &BreadWidget::refresh,
            this, &ObjectsTableWidget::onPathChanged);

    //"桶列表"按钮：重新请求桶列表，数据到了 UiMain 会切回桶页
    connect(ui->buttonBuckets, &QPushButton::clicked, this, []{
        MG->mGate->send(API::BUCKET::LIST, QJsonValue());
    });

    //设置每页可以选择几条
    ui->widgetPage->setMaxRowList({5, 10, 20});

    //翻页时就隐藏不在这一页的行
    connect(ui->widgetPage, &PageWidget::pageNumChanged,
            this, &ObjectsTableWidget::onPageNumChanged);

    //刷新.按面包屑当前路径重新取一遍
    connect(ui->buttonRefresh, &QPushButton::clicked, this, [this]{
        onPathChanged(ui->widgetBread->currentPath());
    });
}

ObjectsTableWidget::~ObjectsTableWidget()
{
    delete ui;
}

void ObjectsTableWidget::onTableViewDoubleClicked(const QModelIndex &index)
{
    //取出隐藏口袋的整个对象
    CloudObject obj = index.siblingAtColumn(0).data(Qt::UserRole).value<CloudObject>();

    //文件不处理->目录更深入一层
    if(!obj.isDir())return;

    //新目录=当前目录+文件夹名
    QJsonObject tmp_obj;
    tmp_obj["bucketName"] = MG->mCloud->currentBucket();
    tmp_obj["location"] = MG->mCloud->currentLocation();
    tmp_obj["dir"] = obj.dir + obj.name;
    MG->mGate->send(API::OBJECT::LIST, tmp_obj);
}

void ObjectsTableWidget::onPathChanged(const QString &path)
{
    //"桶/a/b"->["桶", "a", "b"]
    QStringList names = path.split("/", Qt::SkipEmptyParts);

    //去掉桶名, COS 的目录从桶里开始算，桶名为 bucketName 传过去了
    names.removeFirst();

    //剩下的拼成"a/b/"，根目录就是空字符串
    QString dir = names.isEmpty() ? QString() : names.join("/") + "/";


    //新目录=当前目录+文件夹名
    QJsonObject params;
    params["bucketName"] = MG->mCloud->currentBucket();
    params["location"] = MG->mCloud->currentLocation();
    params["dir"] = dir;
    MG->mGate->send(API::OBJECT::LIST, params);
}

void ObjectsTableWidget::onPageNumChanged(int start, int len)
{
    QStandardItemModel* model = MG->mCloud->modelObjects();

    for(int i=0; i < model->rowCount(); ++i)
    {

        bool hidden = (i < start || i >= start + len);
        ui->tableView->setRowHidden(i, hidden);
    }

}