#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QElapsedTimer>

namespace Ui {
class ProgressWidget;
}

class ProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWidget(QWidget *parent = nullptr);
    ~ProgressWidget();

    //设置总字节数，然后开始计时，minValue 代表开始位置， maxValue 代表结束位置
    void setRange(int64_t minValue, int64_t maxValue);

    //设置已经传的字节
    void setValue(int64_t value);

    //停止计时
    void stop();

    //传输成功
    void setFinished(const QString& msg);

    //传输失败
    void setError(const QString& msg);

private:
    Ui::ProgressWidget *ui;
    //计时器,一个进度条一个
    QElapsedTimer time_;
    //文件的总字节数
    int64_t total_ = 0;
};

#endif // PROGRESSWIDGET_H
