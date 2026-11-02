#ifndef CREATEBUCKETDIALOG_H
#define CREATEBUCKETDIALOG_H

#include "fend/uicom/qosdialog.h"
#include "middle/models/bucket.h"

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

protected:
    //窗口移动就把打开的名单收起来
    void moveEvent(QMoveEvent* event)override;

private:
    Ui::CreateBucketDialog *ui_create;   //避免遮住父类的ui,更改名字
    QString appId_; //账号编号->从现有桶借来
};

#endif //CREATEBUCKETDIALOG_H
