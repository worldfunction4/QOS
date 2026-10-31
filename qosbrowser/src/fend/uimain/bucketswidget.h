#ifndef BUCKETSWIDGET_H
#define BUCKETSWIDGET_H

#include <QWidget>

namespace Ui {
class BucketsWidget;
}

class BucketsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BucketsWidget(QWidget *parent = nullptr);
    ~BucketsWidget();

    //获取上次选中的桶
    void selectBucket(const QString& name);

private:
    Ui::BucketsWidget *ui;

    //记住上次打开的桶，刷新后还用它
    QString lastBucket_;
};

#endif // BUCKETSWIDGET_H
