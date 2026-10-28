#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class PageWidget;
}

//翻页控件：显示总条数、每页条数、当前页，并通知外部切换页
class PageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PageWidget(QWidget *parent = nullptr);
    ~PageWidget();

    //获取属性
    int totalRow() const;
    int maxRow() const;
    int currentPage() const;
    int pageCount() const;
    bool isFirstPage() const;
    bool isLastPage() const;

    //设置总条数，会回到第1页
    void setTotalRow(int total);

    //设置每页条数，必须是可选列表里的值，会回到第1页
    void setMaxRow(int rows);

    //设置每页条数的可选列表，比如 {10, 20, 50}
    void setMaxRowList(const QList<int> &rowList);

signals:
    //页码变化：显示从第 start 条开始的 len 条
    void pageNumChanged(int start, int len);

public slots:
    void next();
    void pre();
    void jumpTo(int page);

private:
    //根据总页数刷新页码列表和 "/N"
    void updatePageCount();

    //回到第1页
    void reset();

    //切换到某一页，并发出信号
    void switchPage(int page);

private:
    Ui::PageWidget *ui;

    //总共多少条
    int totalRow_ = 0;

    //每页多少条
    int maxRow_ = 10;

    //当前第几页，从1开始
    int currentPage_ = 1;

    //每页条数的可选列表
    QList<int> maxRowList_;
};

#endif // PAGEWIDGET_H