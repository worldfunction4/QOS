#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>

namespace Ui {
class ToolBarWidget;
}

class ToolBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *parent = nullptr);
    ~ToolBarWidget();
signals:
    void loginout();

private slots:
    void on_loginoutButton_clicked();

private:
    Ui::ToolBarWidget *ui;
};

#endif // TOOLBARWIDGET_H
