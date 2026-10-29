#ifndef OBJECTSTABLEWIDGET_H
#define OBJECTSTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class ObjectsTableWidget;
}

class ObjectsTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectsTableWidget(QWidget *parent = nullptr);
    ~ObjectsTableWidget();

private slots:
    //双击对象->如果是目录->进入
    void onTableViewDoubleClicked(const QModelIndex& index);
    //面包屑跳转或刷新->按路径重新取对象
    void onPathChanged(const QString& path);

private:
    Ui::ObjectsTableWidget *ui;
};

#endif // OBJECTSTABLEWIDGET_H
