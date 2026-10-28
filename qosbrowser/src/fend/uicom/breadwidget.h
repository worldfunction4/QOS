#ifndef BREADWIDGET_H
#define BREADWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class BreadWidget;
}

class BreadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BreadWidget(QWidget *parent = nullptr);
    ~BreadWidget();

    void setPath(const QString& path);
    void setPath(const QStringList& names);
    void clear();

    //获取完整目录
    QString currentPath()const;
    //获取根目录，也就是桶的名字
    QString rootName()const;
    //判断
    bool isEmpty()const;  //一格都没有
    bool isRoot()const;   //只剩下桶名字

signals:
    void refresh(const QString &path);       // 点了最后一格：刷新当前层
    void pathChanged(const QString &path);   // 点了前面的格子：跳到那一层

private:
    //追加一格
    void addNameButton(const QString& name);
    //处理点击
    void onItemClicked(const QModelIndex &index);
    //最后一格
    QStandardItem* currentItem()const;
    //从第一格接到item
    QString getPath(QStandardItem* item = nullptr)const;

private:
    Ui::BreadWidget *ui;
    QString rootName_; //桶名字
    QStandardItemModel* model_; //承载名字的容器
};

#endif // BREADWIDGET_H
