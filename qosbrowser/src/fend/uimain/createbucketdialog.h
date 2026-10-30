#ifndef CREATEBUCKETDIALOG_H
#define CREATEBUCKETDIALOG_H

#include "src/fend/uicom/qosdialog.h"
#include "src/middle/models/bucket.h"

namespace Ui { class CreateBucketDialog; }

//创建桶弹窗：填桶名和地域
class CreateBucketDialog : public QosDialog
{
    Q_OBJECT
public:
    explicit CreateBucketDialog(QWidget *parent = nullptr);
    ~CreateBucketDialog();

    //取出用户填的桶名和地域
    Bucket bucket() const;

private:
    Ui::CreateBucketDialog *ui_create;   //避免遮住父类的ui,更改名字
};

#endif //CREATEBUCKETDIALOG_H