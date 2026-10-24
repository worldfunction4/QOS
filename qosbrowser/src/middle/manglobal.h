#ifndef MANGLOBAL_H
#define MANGLOBAL_H

//使用宏更加方便
#define MG ManGlobal::instance()

#include <QObject>

//使用前向声明，需要什么再进行include
class ManPlugin;
class ManSignals;
class ManCloud;
class ManDB;
class LoggerProxy;
class Gateway;

class ManGlobal : public QObject
{
    Q_OBJECT
public:
    explicit ManGlobal(QObject* parent=nullptr);
    ~ManGlobal();
    static ManGlobal* instance();
    void init(int argc, char* argv[]);
public:
    //原本单例改为当前单例成员变量
    ManPlugin* mPlugin = nullptr;
    ManSignals* mSignal = nullptr;
    ManCloud* mCloud = nullptr;
    ManDB* mDb = nullptr;
    LoggerProxy* mLog = nullptr;
    Gateway* mGate = nullptr;
};

#endif // MANGLOBAL_H
