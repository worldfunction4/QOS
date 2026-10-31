// qosdialog.cpp
#include "qosdialog.h"
#include "ui_qosdialog.h"
#include "ui_basedialog.h"   //访问父类的labelLogo、labelTitle
#include "config/global.h"

QosDialog::QosDialog(QWidget *parent)
    : BaseDialog(parent)
    , ui_qos(new Ui::QosDialog)
{
    ui_qos->setupUi(body());   //把自己的界面放进父类的内容区
    setLogo(GLOBAL::IMG::LOGO);
    setTitle("Qos");
    ui->labelLogo->setFixedSize(32, 24);       //按比例缩小
    ui->labelTitle->setProperty("style_font", "h4"); //套用 qss 里的h4粗体样式
}

QosDialog::~QosDialog()
{
    delete ui_qos;
}
