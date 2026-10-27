#ifndef MANSIGNALS_H
#define MANSIGNALS_H

#include "middle/models/bucket.h"
#include "middle/models/cloudobject.h"
#include <QObject>

class ManSignals : public QObject
{
    Q_OBJECT
public:
    explicit ManSignals(QObject *parent = nullptr);
signals:
    //登录信号，error可以为通用信号
    void loginSuccess();
    void error(int api, const QString& msg);
    void bucketsSuccess(const QList<Bucket>& buckets);

    //对象信号
    void objectsSuccess(const QList<CloudObject>& object);

    //操作信号
    //下载进度信号(任务id， 已传大小，总大小)
    void downloadProcess(const QString& workId, long long transferred, long long total);
    //下载成功信号
    void downloadSuccess(const QString& workId);
    //上传进度信号
    void uploadProcess(const QString& workId, long long transferred, long long total);
    //上传成功信号
    void uploadSuccess(const QString& workId);

};

#endif // MANSIGNALS_H
