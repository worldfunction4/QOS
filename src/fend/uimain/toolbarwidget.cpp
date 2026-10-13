#include "toolbarwidget.h"
#include "ui_toolbarwidget.h"

ToolBarWidget::ToolBarWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBarWidget)
{
    ui->setupUi(this);
}

ToolBarWidget::~ToolBarWidget()
{
    delete ui;
}

void ToolBarWidget::on_loginoutButton_clicked()
{
    emit loginout();
}


void ToolBarWidget::on_uploadButton_clicked()
{
    //上传一个文件
    //QString path = QFileDialog::getOpenFileName(this, "上传", "C:\\Users");

    //上传多个文件
    // QStringList path_list = QFileDialog::getOpenFileNames(this, "上传", "C:", "txt(*.txt);;hpp(*.h, *.cpp)");
    // for(const QString& path : path_list)
    // {
    //     qDebug() << path;
    // }

    //上传文件夹
    QString path = QFileDialog::getExistingDirectory(this, "上传", "D:");
    qDebug() << path;
}


void ToolBarWidget::on_downButton_clicked()
{
    //下载一个文件
    QString path = QFileDialog::getSaveFileName(this, "下载", "C:\\Users");
    qDebug() << path;
}

