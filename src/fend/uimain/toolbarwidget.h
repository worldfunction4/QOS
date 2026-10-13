#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QStringList>

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
    void on_uploadButton_clicked();

    void on_downButton_clicked();

private:
    Ui::ToolBarWidget *ui;
};

#endif // TOOLBARWIDGET_H
