#include "tableitemdelegate.h"
#include <QAbstractItemView>
#include <QScrollBar>
#include <QEvent>
#include <QCursor>
#include <QMouseEvent>

TableItemDelegate::TableItemDelegate(QAbstractItemView *table, QObject *parent)
    :QStyledItemDelegate{parent ? parent : table}
    ,table_(table)
{
    //表格还没建立
    if(!table_ || !table_->viewport())return;

    // -1 表示没有悬停行。画格子时 Qt 有时把 widget 指到表格，有时指到视口，两边都写
    table_->setProperty("hover_row", -1);
    table_->viewport()->setProperty("hover_row", -1);

    //鼠标在视口上移动或离开时，Qt先调用本对象的 eventFilter，再把消息交给视口
    table_->viewport()->installEventFilter(this);

    //上下滚动按照光标重新算行号
    connect(table_->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &TableItemDelegate::syncHoverFromCursor);

    //点表头排序->也要重算
    if(table_->model())
    {
        connect(table_->model(), &QAbstractItemModel::layoutChanged,
                this, &TableItemDelegate::syncHoverFromCursor);
    }
}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //拷贝修改悬停状态
    QStyleOptionViewItem opt = option;

    //如果和鼠标在同一行就悬停
    if(index.isValid() && index.row() == hoverRow_)
    {
        opt.state |= QStyle::State_MouseOver;
    }
    QStyledItemDelegate::paint(painter, opt, index);
}

void TableItemDelegate::applyHover(QStyleOptionViewItem *option, const QModelIndex &index)
{
    //参数不齐->保持原样
    if(!option || !index.isValid() || !option->widget)return;

    const QVariant hover = option->widget->property("hover_row");
    if(!hover.isValid())return;
    if(index.row() != hover.toInt())return;
    option->state |= QStyle::State_MouseOver;
}

bool TableItemDelegate::eventFilter(QObject *watched, QEvent *event)
{
    //其他空间事件照旧下传
    if(!table_ || watched != table_->viewport())
    {
        return QStyledItemDelegate::eventFilter(watched, event);
    }

    //鼠标在格子之间移动
    if(event->type() == QEvent::MouseMove)
    {
        const auto* mouse = static_cast<const QMouseEvent*>(event);
        const QModelIndex index = table_->indexAt(mouse->position().toPoint());
        setHoverRow(index.isValid()? index.row() : -1);

        //返回false
        return false;
    }

    //鼠标离开表格
    if(event->type() == QEvent::Leave)
    {
        setHoverRow(-1);
        return false;
    }

    return QStyledItemDelegate::eventFilter(watched, event);
}

void TableItemDelegate::syncHoverFromCursor()
{
    if(!table_ || !table_->viewport())return;

    //光标在视口的位置
    const QPoint pos = table_->viewport()->mapFromGlobal(QCursor::pos());

    //光标不在表格
    if(!table_->viewport()->rect().contains(pos))
    {
        setHoverRow(-1);
        return;
    }
    const QModelIndex index = table_->indexAt(pos);
    setHoverRow(index.isValid()? index.row() : -1);
}

void TableItemDelegate::setHoverRow(int row)
{
    //鼠标移动后->在同一行不用管
    if(row == hoverRow_)return;

    const int old = hoverRow_;
    hoverRow_ = row;
    if(table_)
    {
        // 地区列的代理从 option.widget 读这个数，widget 可能是表格或视口
        table_->setProperty("hover_row", hoverRow_);
        if(table_->viewport())
            table_->viewport()->setProperty("hover_row", hoverRow_);
    }

    //旧行去掉浅青
    updateRow(old);
    updateRow(hoverRow_);
}

void TableItemDelegate::updateRow(int row) const
{
    if (!table_ || !table_->model() || row < 0) return;
    if (row >= table_->model()->rowCount()) return;

    //用这一行第一格的位置算出整行高度
    const QRect rect = table_->visualRect(table_->model()->index(row, 0));
    if (!rect.isValid()) return;

    //只把这一横条标成要重画
    table_->viewport()->update(0, rect.y(), table_->viewport()->width(), rect.height());
}
