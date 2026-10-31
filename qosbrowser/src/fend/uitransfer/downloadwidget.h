#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QJsonValue>

namespace Ui {
class DownloadWidget;
}

//头文件只需要指针
class ProgressWidget;

class DownloadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWidget(QWidget *parent = nullptr);
    ~DownloadWidget();

private slots:
    //开始下载：新添一行
    void onStartDownload(const QString& workId, const QString& key, const QString& localPath, long long total);
    //下载进度：更新进度条
    void onDownloadProcess(const QString& workId, long long transferred, long long);
    //下载成功：标记完成
    void onDownloadSuccess(const QString& workId);

    //下载失败
    void onDownloadError(int api, const QString& msg, const QJsonValue& req);

private:
    //workid 寻找进度条
    ProgressWidget* getProgress(const QString& workId);
    //设置表头
    void initHead();

private:
    Ui::DownloadWidget *ui;
};

#endif // DOWNLOADWIDGET_H
