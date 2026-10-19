#include "bucketdelegate.h"
#include <QComboBox>

BucketDelegate::BucketDelegate(QObject* parent)
    :QStyledItemDelegate(parent)
{}

QWidget *BucketDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        QComboBox* box = new QComboBox(parent);
        //显示数据与实际分离

        box->addItem("ap-Sichuan", "ap-sichuan");
        box->addItem("ap-Beijing", "ap-beijing");
        box->addItem("ap-Shanghai", "ap-shanghai");
        box->setFrame(false);
        return box;
}

void BucketDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        //获取原本内容
        QString text = index.data(Qt::EditRole).toString();
        QComboBox* box = static_cast<QComboBox*>(editor);
        int position = box->findData(text);
        if(position >= 0)
        {
            box->setCurrentIndex(position);
        }
}

void BucketDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

        QComboBox* box = static_cast<QComboBox*>(editor);
        model->setData(index, box->currentData(), Qt::EditRole);
}
