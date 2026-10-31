#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>

namespace Ui {
class UploadWidget;
}

//头文件只需要指针
class ProgressWidget;

class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadWidget(QWidget *parent = nullptr);
    ~UploadWidget();

private slots:
    //上传
    void onStartUpload(const QString& workId,
                       const QString& key,
                       const QString& localPath,
                       long long total);

    //上传进度
    void onUploadProcess(const QString& workId,
                         long long transferred,
                         long long);

    //上传成功
    void onUploadSuccess(const QString& workId);
private:
    //获取进度条->返回指针类型
    ProgressWidget* getProgress(const QString& workId);
    //表头
    void initHead();

private:
    Ui::UploadWidget *ui;
};

#endif // UPLOADWIDGET_H
