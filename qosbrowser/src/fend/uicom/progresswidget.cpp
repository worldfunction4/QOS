#include "progresswidget.h"
#include "helper/bytehelper.h"
#include "ui_progresswidget.h"

ProgressWidget::ProgressWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProgressWidget)
{
    ui->setupUi(this);
}

ProgressWidget::~ProgressWidget()
{
    delete ui;
}

void ProgressWidget::setRange(int64_t minValue, int64_t maxValue)
{
    //总大小
    total_ = maxValue - minValue;
    time_.start();

    //进度条只存百分比0-100
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
}

void ProgressWidget::setValue(int64_t value)
{
    if(total_ <= 0)
    {
        ui->progressBar->setValue(100);
        return;
    }

    //计算百分比
    ui->progressBar->setValue(static_cast<int>(value * 100 / total_));

    //过去几秒？
    double s = time_.elapsed() / 1000.0;
    if(s <= 0)return;

    //平均速度
    double speed = value/s;

    //转为 kb/mb
    ui->labelSpeed->setText(ByteHelper::toBeautifulStr(static_cast<uint64_t>(speed)) + "/s");

    //剩余字节
    int64_t l_value = total_ - value;

    if(speed > 0 && l_value > 0)
    {
        //剩余s=剩余字节/速度
        int64_t l_s = static_cast<int64_t>(l_value / speed);
        ui->labelTime->setText(QString("剩余:%1s").arg(l_s));
    }
    else
    {
        ui->labelTime->setText("");
    }
}

void ProgressWidget::stop()
{
    //计时器失效
    time_.invalidate();
    ui->labelSpeed->clear();
}

void ProgressWidget::setFinished(const QString &msg)
{
    //进度条100%
    ui->progressBar->setValue(100);
    //显示文字
    ui->labelTime->setText(msg);
    stop();
    //传完了后把速度隐藏
    ui->labelSpeed->hide();
    //一个标签显示整句话
    ui->labelTime->setText(msg);
}

void ProgressWidget::setError(const QString &msg)
{
    //停止->显示错误原因
    stop();
    ui->labelTime->setText(msg);
}
