#include "qosmessagebox.h"
#include "ui_qosmessagebox.h"

#include <QPushButton>

QosMessageBox::QosMessageBox(QWidget *parent)
    : QosDialog(parent)
    , ui_box(new Ui::QosMessageBox)
{
    //界面放进父类内容区
    ui_box->setupUi(body());
}

QosMessageBox::~QosMessageBox()
{
    delete ui_box;
}

QString QosMessageBox::showMessage(const QString &title, const QString &text, const QStringList &buttonTexts)
{
    //先清空上次的结果
    clickedText_ = "";
    //标题
    setTitle(title);
    //提示文字
    ui_box->label->setText(text);
    //创建按钮
    createButtons(buttonTexts);
    //模态显示，用户点按钮或关闭窗口前，代码停在这里
    exec();
    //返回点了哪个按钮，直接关窗口就是空字符串
    return clickedText_;
}

void QosMessageBox::createButtons(const QStringList &buttonTexts)
{
    for(const QString& text : buttonTexts)
    {
        //每个文字创建一个按钮
        QPushButton* button = new QPushButton(text, this);
        //点击：记下文字，然后关闭窗口
        connect(button, &QPushButton::clicked, this, [this, text]{
            clickedText_ = text;
            accept();
        });
        //放进下面那排
        ui_box->horizontalLayout->addWidget(button);
    }
}