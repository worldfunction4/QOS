#include "pagewidget.h"
#include "ui_pagewidget.h"

PageWidget::PageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageWidget)
{
    ui->setupUi(this);

    //点击上一页、下一页
    connect(ui->buttonPre, &QPushButton::clicked, this, &PageWidget::pre);
    connect(ui->buttonNext, &QPushButton::clicked, this, &PageWidget::next);

    //两个输入框只能从下拉列表里选，不能手动打字，文字居中
    ui->lineMaxRows->setReadOnly(true);
    ui->lineMaxRows->setAlignment(Qt::AlignHCenter);
    ui->lineCurrentPage->setReadOnly(true);
    ui->lineCurrentPage->setAlignment(Qt::AlignHCenter);

    //选择了每页条数
    connect(ui->lineMaxRows, &ComboLine::itemSelected, this, [=](const QString &text){
        setMaxRow(text.toInt());
    });

    //选择了页码
    connect(ui->lineCurrentPage, &ComboLine::itemSelected, this, [=](const QString &text){
        jumpTo(text.toInt());
    });

    //默认每页可选 10、20、50 条
    setMaxRowList({10, 20, 50});

    //初始状态：0条数据
    setTotalRow(0);
}

PageWidget::~PageWidget()
{
    delete ui;
}


int PageWidget::totalRow() const
{
    return totalRow_;
}

int PageWidget::maxRow() const
{
    return maxRow_;
}

int PageWidget::currentPage() const
{
    return currentPage_;
}

int PageWidget::pageCount() const
{
    //没有数据时也显示1页
    if (totalRow_ <= 0)
        return 1;

    //整数向上取整：53条每页10条，(53+9)/10 = 6页
    return (totalRow_ + maxRow_ - 1) / maxRow_;
}

bool PageWidget::isFirstPage() const
{
    return currentPage_ == 1;
}

bool PageWidget::isLastPage() const
{
    return currentPage_ == pageCount();
}

void PageWidget::setTotalRow(int total)
{
    totalRow_ = qMax(0, total);

    //目前显示"共53条，每页"
    ui->labelTotal->setText(QString("共%1条，每页").arg(totalRow_));

    reset();
}

void PageWidget::setMaxRow(int rows)
{
    //不在可选列表里的值直接忽略
    if (!maxRowList_.contains(rows))
        return;

    maxRow_ = rows;
    ui->lineMaxRows->setText(QString::number(rows));
    reset();
}

void PageWidget::setMaxRowList(const QList<int> &rowList)
{
    if (rowList.isEmpty())
        return;

    maxRowList_ = rowList;

    //把数字转成文字，交给下拉提示
    QStringList words;
    for (int n : rowList)
        words << QString::number(n);
    ui->lineMaxRows->setWords(words);

    //默认选第一个
    setMaxRow(rowList.first());
}

void PageWidget::next()
{
    jumpTo(currentPage_ + 1);
}

void PageWidget::pre()
{
    jumpTo(currentPage_ - 1);
}

void PageWidget::jumpTo(int page)
{
    //超出范围或者就是当前页，什么都不做
    if (page < 1 || page > pageCount() || page == currentPage_)
        return;

    switchPage(page);
}

void PageWidget::updatePageCount()
{
    int count = pageCount();

    //显示 "/6"
    ui->labelPageCount->setText("/" + QString::number(count));

    //页码下拉列表：1 到 count
    ui->lineCurrentPage->setWords(1, count);
}

void PageWidget::reset()
{
    updatePageCount();
    switchPage(1);
}

void PageWidget::switchPage(int page)
{
    currentPage_ = page;
    ui->lineCurrentPage->setText(QString::number(page));

    //第一页禁用上一页，最后一页禁用下一页
    ui->buttonPre->setDisabled(isFirstPage());
    ui->buttonNext->setDisabled(isLastPage());

    //算出这一页从第几条开始、有几条
    int start = (page - 1) * maxRow_;
    int len = qMin(maxRow_, totalRow_ - start);
    if (len < 0)
        len = 0;

    emit pageNumChanged(start, len);
}