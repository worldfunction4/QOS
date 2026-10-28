#ifndef COMBOLINE_H
#define COMBOLINE_H

#include <QLineEdit>
#include <QStringList>

//点击即弹出提示列表的输入框
class ComboLine : public QLineEdit
{
    Q_OBJECT

public:
    explicit ComboLine(QWidget *parent = nullptr);
    explicit ComboLine(const QStringList &words,
                       QWidget *parent = nullptr);

    //设置提示列表
    void setWords(const QStringList &words);

    //设置数字提示列表，从 start 开始一共 count 个，翻页控件会用到
    void setWords(int start, int count);

    //获取提示列表
    QStringList words() const;

signals:
    //提示列表中的某一项被选中
    void itemSelected(const QString &text);

protected:
    //鼠标按下时弹出提示列表
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // COMBOLINE_H