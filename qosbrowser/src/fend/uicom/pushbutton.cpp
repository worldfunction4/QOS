#include "pushbutton.h"

#include <QEnterEvent>

PushButton::PushButton(QWidget *parent)
    : QPushButton(parent)
{}

void PushButton::enterEvent(QEnterEvent *event)
{
    //交给父类处理进入
    QPushButton::enterEvent(event);
    //光标改成手型
    setCursor(Qt::PointingHandCursor);
}

void PushButton::leaveEvent(QEvent *event)
{
    //交给父类处理离开
    QPushButton::leaveEvent(event);
    //光标改回箭头
    setCursor(Qt::ArrowCursor);
}