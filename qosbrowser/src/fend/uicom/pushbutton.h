#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class QEnterEvent;
class QEvent;

//鼠标放上去会变成手型的按钮
class PushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PushButton(QWidget *parent = nullptr);

protected:
    //鼠标进入
    void enterEvent(QEnterEvent *event) override;

    //鼠标离开
    void leaveEvent(QEvent *event) override;
};

#endif