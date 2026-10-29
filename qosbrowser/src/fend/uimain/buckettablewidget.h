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

private:
    Ui::BucketTableWidget *ui;
};

#endif // BUCKETTABLEWIDGET_H
