#ifndef APIS_H
#define APIS_H

namespace API
{
    namespace LOGIN
    {
        inline const int NORMAL = 1;
    }

    //接口留空比较多是为了方便以后添加新功能
    //关于桶
    namespace BUCKET
    {
        inline const int BASE = 100;
        inline const int LIST = BASE + 1; //获取桶列表
        inline const int PUT  = BASE + 2; //创建桶
        inline const int DEL  = BASE + 3; //删除桶
    }

    //关于对象
    namespace OBJECT
    {
        inline const int BASE = 200;
        inline const int LIST = BASE + 1; //获取对象列表
        inline const int GET  = BASE + 2; //下载对象
        inline const int PUT  = BASE + 3; //上传对象
    }
}

#endif // APIS_H
