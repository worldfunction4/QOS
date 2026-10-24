#ifndef MANSIGNALS_H
#define MANSIGNALS_H

#include <QObject>

class ManSignals : public QObject
{
    Q_OBJECT
public:
    explicit ManSignals(QObject *parent = nullptr);
signals:
    void loginSuccess();
    void error(int api, const QString& msg);
};

#endif // MANSIGNALS_H
