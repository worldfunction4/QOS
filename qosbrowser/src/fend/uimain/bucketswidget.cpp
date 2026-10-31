#include "bucketswidget.h"
#include "ui_bucketswidget.h"
#include "src/bend/man/mancloud.h"
#include "src/middle/manglobal.h"
#include "src/bend/gateway.h"
#include "src/config/apis.h"
#include "src/middle/signals/mansignals.h"

#include <QStandardItemModel>
#include <QJsonObject>
#include <QLineEdit>

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

    //桶列表到了信号连接到输入框显示下拉
    connect(MG->mSignal, &ManSignals::bucketsSuccess, this, [this]{
        QStringList words;
        QStandardItemModel* model = MG->mCloud->modelBuckets();
        for(int i=0; i<model->rowCount();++i)
        {
            words << model->index(i, 0).data().toString();
        }
        ui->lineSearch->setWords(words);
    });

    //打字时过滤不符合的内容
    connect(ui->lineSearch, &QLineEdit::textEdited, this, [this](const QString& text){
        QStandardItemModel* model = MG->mCloud->modelBuckets();
        for(int i=0; i < model->rowCount();++i)
        {
            bool hide = !model->index(i, 0).data().toString().contains(text);
            ui->listView->setRowHidden(i, hide);
        }
    });

    //下拉里选中桶=双击
    connect(ui->lineSearch, &ComboLine::itemSelected, this, [this](const QString& name){
        QStandardItemModel* model = MG->mCloud->modelBuckets();
        for(int i=0; i < model->rowCount(); ++i)
        {
            if(model->index(i, 0).data().toString() == name)
            {
                emit ui->listView->doubleClicked(model->index(i, 0));
                break;
            }
        }
    });

    //搜索按钮->输入框的文字打开同名桶
    connect(ui->buttonSearch, &QPushButton::clicked, this, [this]{
        emit ui->lineSearch->itemSelected(ui->lineSearch->text().trimmed());
    });


    //对象列表到了->记住当前桶，并在左边选中它
    connect(MG->mSignal, &ManSignals::objectsSuccess, this, [this]{
        //记下当前打开的桶
        lastBucket_ = MG->mCloud->currentBucket();
        //按名字选中
        selectBucket(lastBucket_);
    });

    //桶列表重新到了（返回桶列表/刷新）->按记住的名字恢复左边选中
    connect(MG->mSignal, &ManSignals::bucketsSuccess, this, [this]{
        selectBucket(lastBucket_);
    });

    //退出登录->忘掉上次的桶，并且清除选中
    connect(MG->mSignal, &ManSignals::unLogin, this, [this]{
        lastBucket_.clear();
        ui->listView->clearSelection();
    });
}

BucketsWidget::~BucketsWidget()
{
    delete ui;
}

void BucketsWidget::selectBucket(const QString &name)
{
    //先清掉旧的选中
    ui->listView->clearSelection();
    //没有记住的桶就什么都不选
    if(name.isEmpty()) return;
    QStandardItemModel* model = MG->mCloud->modelBuckets();
    //遍历找到同名的那一行
    for(int i = 0; i < model->rowCount(); ++i)
    {
        if(model->index(i, 0).data().toString() == name)
        {
            //设为当前行并选中
            ui->listView->setCurrentIndex(model->index(i, 0));
            break;
        }
    }
}