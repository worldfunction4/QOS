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
