#include "createbucketdialog.h"
#include "ui_createbucketdialog.h"

#include "src/fend/uicom/qosmessagebox.h"

CreateBucketDialog::CreateBucketDialog(QWidget *parent)
    :QosDialog{parent}
    ,ui_create(new Ui::CreateBucketDialog)
{
    ui_create->setupUi(body());
    //确定按钮用主按钮样式
    ui_create->buttonOk->setProperty("style_button", "main");

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
    b.location = ui_create->lineLocation->text().trimmed();
    return b;
}


