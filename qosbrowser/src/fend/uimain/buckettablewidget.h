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

private:
    Ui::BucketTableWidget *ui;
};

#endif // BUCKETTABLEWIDGET_H
