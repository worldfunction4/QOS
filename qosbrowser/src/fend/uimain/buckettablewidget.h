#ifndef BUCKETTABLEWIDGET_H
#define BUCKETTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class BucketTableWidget;
}

class BucketTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BucketTableWidget(QWidget *parent = nullptr);
    ~BucketTableWidget();

private slots:
    //双击桶->获取相应对象列表
    void onTableViewDoubleClicked(const QModelIndex& index);

    //和对象列表一样显示从 start 的 len 行
    void onPageNumChanged(int start, int len);

private:
    Ui::BucketTableWidget *ui;
};

#endif // BUCKETTABLEWIDGET_H
