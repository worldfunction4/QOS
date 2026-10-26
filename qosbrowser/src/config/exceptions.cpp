#include "exceptions.h"
#include "src/helper/filehelper.h"

#include <QList>
#include <QStringList>

BaseException::BaseException(const QString& code,
                             const QString& message)
    :code_(code), msg_(message)
{}

ErrorMap BaseException::parseErrorCode(const QString &csv_path)
{
    ErrorMap map;

    //拿到csv文件
    QList<QStringList> csv = FileHelper::readAllCsv(csv_path);
    //跳过表头
    if(!csv.isEmpty())csv.removeFirst();
    //遍历
    for(const QStringList& row : csv)
    {
        QString code;
        QString msg;
        //如果列数不够呢？直接跳过这一行
        if(row.size() < 7)continue;
        for(int i=0; i < 6; ++i)
        {
            if(row[i].isEmpty())
            {
                code += "0";
            }
            else
            {
                QStringList parts = row[i].split("_");
                msg += parts.at(0);
                code += parts.at(1);
            }
        }
        //补充说明不为空的情况
        if(!row[6].isEmpty())
        {
            msg = row[6];
        }
        //存入表中
        map.insert(code, msg);
    }
    return map;
}

void BaseException::generateErrorCodeFile(const QString &csvPath, const QString &targetPath)
{
    QStringList rows;
    //头文件开头：防止重复包含
    rows << "#ifndef ERRORCODE_H" << "#define ERRORCODE_H" << "";

    //解析 csv 得到 code->msg 表，每一项生成一行宏
    ErrorMap errors = parseErrorCode(csvPath);
    for(auto it = errors.cbegin(); it != errors.cend(); ++it)
    {
        //比如：#define EC_211100 "211100"  // 用户登录账号密码有问题
        rows << QString("#define EC_%1 \"%1\"  // %2").arg(it.key(), it.value());
    }

    //头文件结尾
    rows << "" << "#endif // ERRORCODE_H";

    //写进目标文件
    FileHelper::writeFile(rows, targetPath);
}

QString BaseException::msg() const
{
    //第一次调用就生成
    //错误码表读取失败也不能再抛异常，否则原始错误会被吞掉
    static ErrorMap map = []{
        try { return BaseException::parseErrorCode(":/resource/static/docs/errorcode.csv"); }
        catch (...) { return ErrorMap{}; }
    }();

    //使用value函数，代表查到就返回对应的值，查不到就返回第二个参数
    QString desc = map.value(code_, "未知错误");
    QString text = QString("错误(%1): %2").arg(code_).arg(desc);


    if(!msg_.isEmpty())
    {
        text += "\n详情：" + msg_;
    }
    return text;
}

QString BaseException::code() const
{
    return code_;
}
