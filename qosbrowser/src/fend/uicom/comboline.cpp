#include "comboline.h"

#include <QCompleter>
#include <QAbstractItemView>

//委托给另一个构造函数，传一个空列表
ComboLine::ComboLine(QWidget *parent)
    : ComboLine(QStringList(), parent)
{
}

ComboLine::ComboLine(const QStringList &words, QWidget *parent)
    : QLineEdit(parent)
{
    setWords(words);
}

void ComboLine::setWords(const QStringList &words)
{
    //先删掉旧的提示器，避免每次设置都多出一个
    if (completer())
        completer()->deleteLater();

    QCompleter *com = new QCompleter(words, this);

    //不区分大小写，包含即匹配
    com->setCaseSensitivity(Qt::CaseInsensitive);
    com->setFilterMode(Qt::MatchContains);

    //去掉弹出列表的滚动条
    com->popup()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    com->popup()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //选中某一项时，把它转发成自己的信号
    connect(com, QOverload<const QString &>::of(&QCompleter::activated),
            this, &ComboLine::itemSelected);

    //避免全局样式把弹出列表的高度压为0
    com->popup()->setStyleSheet(
        "QAbstractItemView { background: #ffffff; border: 1px solid #00bcd4; min-height: 30px; outline: 0; }"
    );

    setCompleter(com);
}

void ComboLine::setWords(int start, int count)
{
    //生成 start, start+1等一共 count 个数字
    QStringList strs;
    for (int i = 0; i < count; ++i)
        strs << QString::number(start + i);
    setWords(strs);
}

QStringList ComboLine::words() const
{
    //从提示器的模型里把所有文字读出来
    QStringList strs;
    if (!completer())
        return strs;

    QAbstractItemModel *m = completer()->model();
    for (int i = 0; i < m->rowCount(); ++i)
        strs << m->index(i, 0).data().toString();
    return strs;
}

void ComboLine::mousePressEvent(QMouseEvent *event)
{
    //先让输入框正常处理点击，比如移动光标
    QLineEdit::mousePressEvent(event);

    //再弹出提示列表
    if (completer())
        completer()->complete();
}