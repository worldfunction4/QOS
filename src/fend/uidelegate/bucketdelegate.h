#ifndef BUCKETDELEGATE_H
#define BUCKETDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>

class BucketDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BucketDelegate(QObject* parent=nullptr);
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;
};

#endif // BUCKETDELEGATE_H
