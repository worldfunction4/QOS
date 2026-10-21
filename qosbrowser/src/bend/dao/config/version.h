#ifndef VERSION_H
#define VERSION_H

#include <QString>

//定义版本
class Version
{
public:
    Version()=default;
    virtual ~Version()=default;

    //设置版本，必须重写，顺便声明为抽象类
    virtual void setVersion()=0;

    //获取版本完整版本
    QString version()const;

    //获取数字版本
    QString versionNum()const;

    //获取主版本
    QString major()const;

protected:
    QString m_major; //主版本类型（商业版或个人版），也就是business或者custom
    QString m_dev; //环境（生产环境还是测试环境），同理：alpha，beta，prod

    //版本号，比如1.1.2
    int m_v1=0;
    int m_v2=0;
    int m_v3=0;
};


#endif // VERSION_H
