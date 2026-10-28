#include "breadwidget.h"
#include "ui_breadwidget.h"
#include "src/config/global.h"

BreadWidget::BreadWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BreadWidget)
    , model_(new QStandardItemModel(this))
{
    ui->setupUi(this);

    //显示模型的数据
    ui->listView->setModel(model_);

    //设置为横向排列
    ui->listView->setFlow(QListView::LeftToRight);

    //设置双击无法改名
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //取消垂直滚动条和水平滚动条
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //清除格子的间距
    ui->listView->setSpacing(0);

    //缩小箭头图标
    ui->listView->setIconSize(QSize(12, 12));

    //不显示键盘焦点框
    ui->listView->setFocusPolicy(Qt::NoFocus);

    //设置样式：去掉边框和背景，普通灰色，悬停稍深
    ui->listView->setStyleSheet(
        "QListView { border: none; background: transparent; }"
        "QListView::item { color: #666666; border: none; padding: 0px; padding-right: 2px; }"
        "QListView::item:hover { color: #333333; }"
        "QListView::item:selected { color: #000000; background: transparent; border: none; }"
        );

    //点击格子时处理刷新或跳转
    connect(ui->listView, &QListView::clicked,
            this, &BreadWidget::onItemClicked);

    //固定高度为一行
    setFixedHeight(30);
}

BreadWidget::~BreadWidget()
{
    delete ui;
}

void BreadWidget::setPath(const QString &path)
{
    //“a/b/c->["a","b","c"]”
    setPath(path.split("/", Qt::SkipEmptyParts));
}

void BreadWidget::setPath(const QStringList &names)
{
    if(names.isEmpty())return;

    clear();
    rootName_ = names.first();
    for(const QString &name : names)
    {
        addNameButton(name.trimmed());
    }
}

void BreadWidget::clear()
{
    model_->clear();
    rootName_.clear();
}

QString BreadWidget::currentPath() const
{
    return getPath();
}

QString BreadWidget::rootName() const
{
    return rootName_;
}

bool BreadWidget::isEmpty() const
{
    return model_->rowCount() == 0;
}

bool BreadWidget::isRoot() const
{
    return model_->rowCount() == 1;
}

void BreadWidget::addNameButton(const QString &name)
{
    if(name.isEmpty())return;

    auto* item = new QStandardItem(name);
    if(!isEmpty())
    {
        item->setIcon(QIcon(GLOBAL::IMG::RIGHT));
    }
    model_->appendRow(item);
}

void BreadWidget::onItemClicked(const QModelIndex &index)
{
    QStandardItem *clicked = model_->itemFromIndex(index);

    if (clicked == currentItem())
        emit refresh(getPath(clicked));       // 点的是当前层就刷新
    else
        emit pathChanged(getPath(clicked));
}

QStandardItem *BreadWidget::currentItem() const
{
    return model_->item(model_->rowCount()-1);
}

QString BreadWidget::getPath(QStandardItem *item)const
{
    if(!item)
    {
        item = currentItem();
    }
    //如果一格都没有？
    if(!item)
    {
        return QString();
    }

    QStringList names;
    for(int i=0; i <= item->row(); ++i)
    {
        names << model_->item(i)->text();
    }
    return names.join("/");
}
