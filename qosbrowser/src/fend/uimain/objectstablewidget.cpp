#include "objectstablewidget.h"
#include "bend/man/mancloud.h"
#include "helper/filehelper.h"
#include "ui_objectstablewidget.h"
#include "src/middle/manglobal.h"
#include "src/middle/signals/mansignals.h"
#include "src/middle/manglobal.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"

#include <QJsonObject>
#include <QFileDialog>
#include <QFileInfo>
#include <QUuid>
#include "src/fend/uicom/qosmessagebox.h"

ObjectsTableWidget::ObjectsTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ObjectsTableWidget)
{
    ui->setupUi(this);
    //返回桶列表按钮用主按钮样式
    ui->buttonBuckets->setProperty("style_button", "main");

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

    //上传。选择本地文件->上传到当前桶的当前目录
    connect(ui->buttonUpload, &QPushButton::clicked, this, [this]{
        //记住上次打开的文件夹
        static QString last_dir = "./";

        //拿到需要的路径
        QString file_path = QFileDialog::getOpenFileName(this, "上传文件", last_dir);

        //取消或选择的不是文件
        QFileInfo info(file_path);
        if(!info.isFile())return;

        //每一个上传任务唯一编号，用QUuid生成一个几乎不会重复的随机编号
        QString workId = QUuid::createUuid().toString();
        QJsonObject obj;
        obj["workId"] = workId;
        obj["bucketName"] = MG->mCloud->currentBucket();
        obj["location"] = MG->mCloud->currentLocation();

        //云名字=当前目录+文件名
        obj["key"] = MG->mCloud->currentDir() + info.fileName();
        obj["localPath"] = file_path;

        //先通知传输列表加一行，最后一个参数是本地文件大小
        emit MG->mSignal->startUpload(obj["workId"].toString(), obj["key"].toString(), file_path, info.size());

        MG->mGate->send(API::OBJECT::PUT, obj);
        last_dir = info.dir().absolutePath();
    });

    //上传成功->刷新当前的目录->弹出提示
    connect(MG->mSignal, &ManSignals::uploadSuccess, this, [this]{
        onPathChanged(ui->widgetBread->currentPath());
        QosMessageBox(this).showMessage("提示", "上传文件成功");
    });

    //下载，选择表格一个文件->选择保存位置
    connect(ui->buttonDownload, &QPushButton::clicked, this, [this]{
        QModelIndex index = ui->tableView->currentIndex();
        if(!index.isValid())
        {
            QosMessageBox(this).showMessage("下载文件", "请先选择下载的文件...");
            return;
        }

        //取出第0列存的整个对象
        CloudObject obj = index.siblingAtColumn(0).data(Qt::UserRole).value<CloudObject>();
        if(obj.isDir())
        {
            QosMessageBox(this).showMessage("下载文件", "抱歉，只能下载文件不能下载文件夹哦？");
            return;
        }

        static QString last_dir = "./";
        QString file_path = QFileDialog::getSaveFileName(this, "下载文件", FileHelper::joinPath(last_dir, obj.name));

        //点击取消
        if(file_path.isEmpty())return;

        QJsonObject params;
        //存入一个变量
        QString workId = QUuid::createUuid().toString();
        params["workId"] = workId;
        params["bucketName"] = MG->mCloud->currentBucket();
        params["location"] = MG->mCloud->currentLocation();
        params["key"] = obj.dir + obj.name;
        params["localPath"] = file_path;
        //通知传输列表新增一行下载任务
        emit MG->mSignal->startDownload(workId, obj.dir+obj.name, file_path, obj.size);
        MG->mGate->send(API::OBJECT::GET, params);
        last_dir = QFileInfo(file_path).dir().absolutePath();
    });

    //下载成功
    connect(MG->mSignal, &ManSignals::downloadSuccess, this, [this]{
        onPathChanged(ui->widgetBread->currentPath());
        QosMessageBox(this).showMessage("下载任务", "下载文件成功");
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
