#ifndef CLOUDOBJECT_H
#define CLOUDOBJECT_H

#include <QString>
#include <QMetaType>

//存储桶里的对象
struct CloudObject
{
    QString dir;           //所在的目录
    QString name;          //去掉目录前缀后的名字，目前是以“/”结尾
    QString lastModified; //最后修改时间，但目录除外，填"-"
    long long size = 0;   //目录为0,文件有可能超过4g

    bool isDir()const     //判断这个对象是否为目录
    {
        return name.endsWith('/');
    }
};

Q_DECLARE_METATYPE(CloudObject)

#endif // CLOUDOBJECT_H
