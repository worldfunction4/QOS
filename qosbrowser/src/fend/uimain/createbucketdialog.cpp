#include "createbucketdialog.h"
#include "ui_createbucketdialog.h"
#include "src/config/global.h"
#include "src/helper/filehelper.h"
#include "src/fend/uicom/qosmessagebox.h"

#include <QMoveEvent>

CreateBucketDialog::CreateBucketDialog(QWidget *parent)
    :QosDialog{parent}
    ,ui_create(new Ui::CreateBucketDialog)
{
    ui_create->setupUi(body());
    //确定按钮用主按钮样式
    ui_create->buttonOk->setProperty("style_button", "main");

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
    b.name = ui_create->lineName->text().trimmed();
    //取下拉框里藏着的地域编号，不是屏幕上的中文
    b.location = ui_create->comboLocation->currentData().toString();
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


