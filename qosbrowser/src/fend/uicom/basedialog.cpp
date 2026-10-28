#include "basedialog.h"
#include "ui_basedialog.h"
#include "src/config/global.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>

BaseDialog::BaseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BaseDialog)
{
    ui->setupUi(this);
    // 去掉系统标题栏，改用自己画的frameTitle
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    setLogo(":/resource/static/imgs/qtlogo.png");
    ui->labelLogo->setFixedSize(24, 24);

    // 关闭按钮：换成图片，点击 = reject()，等于关闭对话框
    addCloseButton(GLOBAL::IMG::CLOSE, GLOBAL::IMG::CLOSE_HOVER);
    connect(ui->buttonClose, &QPushButton::clicked, this, &BaseDialog::reject);
    setAllButtonSize();
}

BaseDialog::~BaseDialog()
{
    delete ui;
}

void BaseDialog::setTitle(const QString &title)
{
    ui->labelTitle->setText(title);
}

void BaseDialog::setLogo(const QString &path)
{
    ui->labelLogo->setStyleSheet(QString("border-image: url(\"%1\");").arg(path));
}

void BaseDialog::addTitleLine(int w)
{
    //用一个固定宽度灰色标签充当竖线
    QLabel* line = new QLabel;
    line->setFixedSize(w, 15);
    line->setStyleSheet("QLabel{background-color: lightgray;}");
    addWidget(line);
}

void BaseDialog::addMinButton(const QString &path, const QString &hoverPath)
{
    QPushButton *btn = addButton(path, hoverPath);
    connect(btn, &QPushButton::clicked, this, &BaseDialog::showMinimized);
}

void BaseDialog::addMaxButton(const QString &maxPath, const QString &maxHover,
                              const QString &normalPath, const QString &normalHover)
{
    QPushButton *btn = addButton(maxPath, maxHover);

    // 按当前状态换图标：已最大化就显示"还原"图标，否则显示"最大化"图标
    auto updateImage = [=]() {
        isMaximized() ? setButtonImage(btn, normalPath, normalHover)
                      : setButtonImage(btn, maxPath, maxHover);
    };
    connect(btn, &QPushButton::clicked, this, [=]() {
        isMaximized() ? showNormal() : showMaximized();
        updateImage();
    });
    updateImage();
}

QPushButton *BaseDialog::addButton(const QString &path, const QString &hoverPath)
{
    QPushButton *btn = new QPushButton;
    btn->setFixedSize(m_size, m_size);
    setButtonImage(btn, path, hoverPath);
    addWidget(btn);   // 加进布局后，布局会把 frameTitle 设为它的父对象，所以不会内存泄漏
    return btn;
}

void BaseDialog::setButtonImage(QPushButton *btn, const QString &path, const QString &hoverPath)
{
    btn->setStyleSheet(QString("QPushButton{border-image: url(\"%1\");}"
                               "QPushButton:hover{border-image: url(\"%2\");}")
                           .arg(path, hoverPath));
}

void BaseDialog::setAllButtonSize(int w)
{
    const auto btns = ui->frameTitle->findChildren<QPushButton *>();
    for (QPushButton *btn : btns) {
        btn->setFixedSize(w, w);
    }
    m_size = w;   // 记下来，以后addButton也用这个大小
}

void BaseDialog::setKeyDisabled()
{
    installEventFilter(this);
}

void BaseDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_start = event->pos();   // 按下点相对于窗口左上角的位置
    }
    QDialog::mousePressEvent(event);
}

void BaseDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        //窗口新位置 = 原位置 + 鼠标挪动的距离
        move(pos() + event->pos() - m_start);
    }
    QDialog::mouseMoveEvent(event);
}

bool BaseDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this && event->type() == QEvent::KeyPress) {
        int key = static_cast<QKeyEvent *>(event)->key();
        if (key == Qt::Key_Return || key == Qt::Key_Enter || key == Qt::Key_Escape) {
            return true;   //吃掉这个事件，不让 QDialog 自己关掉
        }
    }
    return QDialog::eventFilter(obj, event);
}

QWidget *BaseDialog::body()
{
    return ui->widgetBody;
}

void BaseDialog::addCloseButton(const QString &path, const QString &hoverPath)
{
    ui->buttonClose->setText("");   // 清掉 .ui 里可能留下的文字
    setButtonImage(ui->buttonClose, path, hoverPath);
}

void BaseDialog::addWidget(QWidget *w)
{
    // 插到关闭按钮前面，关闭按钮就永远在最右边
    int i = ui->horizontalLayout->indexOf(ui->buttonClose);
    ui->horizontalLayout->insertWidget(i, w);
}