#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QStyledItemDelegate>

//需要保存一个相应类型指针
class QAbstractItemView;


class TableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableItemDelegate(QAbstractItemView* table, QObject *parent = nullptr);

    //画一格->调用一次
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index)const override;

    //地区列也需要调用
    static void applyHover(QStyleOptionViewItem* option, const QModelIndex& index);

protected:
    //鼠标在表格上移动或者离开表格时，将事件送到此处
    bool eventFilter(QObject* watched, QEvent* event)override;

private slots:
    //滚动或排序,鼠标底下的行也许会改变
    void syncHoverFromCursor();

private:
    //决定要不要重画的函数,如果新的行数和成员变量一样就返回
    void setHoverRow(int row);

    //将某一整行标记为：需要重画，
    void updateRow(int row)const;

    //当前表
    QAbstractItemView* table_ = nullptr;
    //鼠标的位置, -1 表示鼠标不在任何一行
    int hoverRow_ = -1;
};

#endif // TABLEITEMDELEGATE_H
