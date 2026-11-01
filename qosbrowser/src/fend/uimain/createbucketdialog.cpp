#include "createbucketdialog.h"
#include "ui_createbucketdialog.h"
#include "src/config/global.h"
#include "src/helper/filehelper.h"
#include "src/fend/uicom/qosmessagebox.h"
#include "src/middle/manglobal.h"
#include "src/bend/man/mancloud.h"

#include <QStandardItemModel>
#include <QMoveEvent>
#include <algorithm>

CreateBucketDialog::CreateBucketDialog(QWidget *parent)
    :QosDialog{parent}
    ,ui_create(new Ui::CreateBucketDialog)
{
    ui_create->setupUi(body());
    //确定按钮用主按钮样式
    ui_create->buttonOk->setProperty("style_button", "main");

    //左侧桶列表模型->借已有桶的账号编号
    QStandardItemModel* m = MG->mCloud->modelBuckets();

    //判断是否有桶？
    if(m->rowCount() > 0)
    {
        //桶全名：fileName-appid
        QString first = m->index(0, 0).data().toString();

        //取最后一个 -
        appId_ = first.mid(first.lastIndexOf('-') + 1);
    }
    //拿到了就显示在输入框右边
    ui_create->labelAppId->setText(appId_.isEmpty() ? "" : "-" + appId_);
    //用qss的提示文字标签，颜色统一在qss里管
    ui_create->labelAppId->setProperty("style_font", "tip");

    //没拿到->隐藏
    ui_create->labelAppId->setVisible(!appId_.isEmpty());

    //然后读取 exe 的地域表，第一行为表头，丢弃
    try
    {
        QList<QStringList> rows = FileHelper::readAllCsv(GLOBAL::PATH::regionsCosFile());
        if(!rows.isEmpty())
        {
            rows.removeFirst();
        }

        //地区下拉框只加不重复的第0列
        QStringList areas;
        for(const QStringList& row : rows)
        {
            if(row.size() < 3) continue;
            QString area = row.at(0).trimmed();
            if(areas.contains(area)) continue;
            areas << area;
            ui_create->comboArea->addItem(area);
        }

        //左边换成哪个地区，右边就只留下这个地区的城市
        auto fillCities = [this, rows](const QString& area){
            //先清空，避免上次的城市还留着
            ui_create->comboLocation->clear();
            for(const QStringList& row : rows)
            {
                if(row.size() < 3) continue;
                if(row.at(0).trimmed() != area) continue;
                ui_create->comboLocation->addItem(row.at(1).trimmed(), row.at(2).trimmed());
            }
        };

        connect(ui_create->comboArea, &QComboBox::currentTextChanged, this, fillCities);
        //打开窗口时先填当前这个地区
        if(ui_create->comboArea->count() > 0)
        {
            fillCities(ui_create->comboArea->currentText());
        }

    }
    catch (...)
    {
        QosMessageBox(this).showMessage("创建桶", "找不到地域名单");
    }

    //关窗口返回accept，取消返回reject
    //点确定先检查，没填完就提示，窗口不关
    connect(ui_create->buttonOk, &QPushButton::clicked, this, [this]{
        Bucket b = bucket();
        if (b.name.isEmpty() || b.location.isEmpty()) {
            QosMessageBox(this).showMessage("提示", "桶名称和桶地域都要填写");
            return;
        }
        //cos最长50个字符
        if(b.name.size() > 50)
        {
            QosMessageBox(this).showMessage("提示", "桶名称最多50位");
            return;
        }

        //检查字符
        for(const QChar& ch : b.name)
        {
            //a-z
            const bool true1 = (ch >= u'a' && ch <= u'z');

            //0-9
            const bool true2 = (ch >= u'0' && ch <= u'9');

            //是否为：-
            const bool true3 = (ch == u'-');

            //如果都不是
            if(!true1 && !true2 && !true3)
            {
                QosMessageBox(this).showMessage("提示", "桶名称只能用小写字母，数字和 - 组成哦");
                return;
            }
        }

        //- 不能放两头
        if(b.name.startsWith(u'-') || b.name.endsWith(u'-'))
        {
            QosMessageBox(this).showMessage("提示", "桶名称不能以 - 开头和结尾哦");
            return;
        }

        //不能有两个--
        if (b.name.contains(QStringLiteral("--"))) {
            QosMessageBox(this).showMessage("提示", "桶名称不能有连续的 -- 哦");
            return;
        }

        //拿当前左侧桶列表的模型，用来查重名
        QStandardItemModel* m = MG->mCloud->modelBuckets();

        //列表里的桶全名是 "名字-账号编号"，所以拿 "输入的名字-" 当开头去比
        QString prefix = b.name + "-";

        //记录有没有找到重名的桶
        bool isSame = false;

        //遍历左侧桶列表的每一行
        for (int row = 0; row < m->rowCount(); ++row)
        {
            //取出这一行的桶全名
            QString have = m->index(row, 0).data().toString();

            //列表里的完整名字和输入的一模一样，就是重名
            if (have == b.name)
            {
                isSame = true;
                break;
            }

            // 开头对上了，才需要继续看后面
            if (have.startsWith(prefix))
            {
                //取出 "-" 后面剩下的部分
                QString rest = have.mid(prefix.size());

                //剩下的正好是我们的账号编号，这就是重名，编号几位都不影响
                isSame = !appId_.isEmpty() && rest == appId_;

                //找到重名就不用再往下找了
                if (isSame) break;
            }
        }

        //找到重名就提示，窗口不关
        if (isSame)
        {
            QosMessageBox(this).showMessage("提示", "已经有同名的桶了哦");
            return;
        }

        //检查全部通过，关窗口
        accept();
    });
    connect(ui_create->buttonCancel, &QPushButton::clicked,
            this, &CreateBucketDialog::reject);
}

CreateBucketDialog::~CreateBucketDialog()
{
    delete ui_create;
}

Bucket CreateBucketDialog::bucket() const
{
    Bucket b;
    // 取输入框里的桶名，去掉两头空格
    b.name = ui_create->lineName->text().trimmed();
    // 取下拉框里藏着的地域编号，不是屏幕上的中文
    b.location = ui_create->comboLocation->currentData().toString();
    // 有账号编号，并且用户没自己带上，才补上
    if (!b.name.isEmpty() && !appId_.isEmpty() && !b.name.endsWith("-" + appId_))
    {
        // 拼成 "名字-账号编号"
        b.name += "-" + appId_;
    }
    return b;
}

void CreateBucketDialog::moveEvent(QMoveEvent *event)
{
    //让原来的移动处理搞定
    QosDialog::moveEvent(event);

    //如果地域开着就关闭
    ui_create->comboArea->hidePopup();

    //如果城市开着也关起来
    ui_create->comboLocation->hidePopup();
}


