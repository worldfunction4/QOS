#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>
#include <QJsonValue>
#include <QLabel>

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

protected:
    //监听可视区域自己的大小变化
    bool eventFilter(QObject* obj, QEvent* event) override;

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

    //上传失败
    void onUploadError(int api, const QString& msg, const QJsonValue& req);

private:
    //获取进度条->返回指针类型
    ProgressWidget* getProgress(const QString& workId);
    //表头
    void initHead();
    //行数决定提示还是隐藏
    void updateEmptyTip();

private:
    Ui::UploadWidget *ui;
    //表格为空：显示文字
    QLabel* labelEmpty_ = nullptr;
};

#endif // UPLOADWIDGET_H
