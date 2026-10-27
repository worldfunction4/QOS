#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

class QPushButton;

namespace Ui {
class BaseDialog;
}

// 自定义标题栏的弹窗基类：登录、创建桶、提示框等都继承它
class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr);
    ~BaseDialog();

    void setTitle(const QString &title);   // 设置标题文字
    void setLogo(const QString &path);     // 设置左上角 logo

    void addTitleLine(int w = 2); // 标题栏竖分割线
    void addMinButton(const QString &path, const QString &hoverPath);   // 最小化按钮
    void addMaxButton(const QString &maxPath, const QString &maxHover,
                      const QString &normalPath, const QString &normalHover); // 最大化/还原按钮
    QPushButton *addButton(const QString &path, const QString &hoverPath); // 通用：加一个按钮并返回它
    void setButtonImage(QPushButton *btn, const QString &path, const QString &hoverPath); // 设置平时和悬停两张图
    void setAllButtonSize(int w = 20);     // 标题栏按钮统一大小
    void setKeyDisabled();                 // 屏蔽回车和 Esc

protected:
    // 没有系统标题栏，拖动窗口要自己实现
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    QWidget *body();                       // 内容区，子类往里放控件

    Ui::BaseDialog *ui;                    // 改成 protected，子类才能用

private:
    void addCloseButton(const QString &path, const QString &hoverPath);
    void addWidget(QWidget *w);            // 插到关闭按钮左边

    QPoint m_start;                        // 鼠标按下时在窗口内的位置
    int m_size = 20;                       // 按钮边长
};

#endif // BASEDIALOG_H