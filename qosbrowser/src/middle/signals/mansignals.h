#ifndef MANSIGNALS_H
#define MANSIGNALS_H

#include "middle/models/bucket.h"
#include <QObject>

class ManSignals : public QObject
{
    Q_OBJECT
public:
    explicit ManSignals(QObject *parent = nullptr);
signals:
    void loginSuccess();
    void error(int api, const QString& msg);
    void bucketsSuccess(const QList<Bucket>& buckets);
};

#endif // MANSIGNALS_H
