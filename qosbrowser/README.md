# Qos
基于Qt6的Widget，使用cmkae构建系统的桌面端对象存储管理器

## 目前的功能
- 登录窗口与主窗口
- 主窗口左右分栏布局
- Mock数据读取展示
- QSS全局样式主题
- 本地SQLite数据库,运行程序时自动创建登录信息表`login_info`
- "记住会话"被勾选时保存登录信息，不勾选则不会保存（删除）

## 技术栈
- C++17
- Qt 6 Widget
- Qt SQL
- CMake

## 资源文件
所有资源使用`resources.qrc`进行管理

```text
resource/static/imgs/       # 图标资源
resource/static/qss/        # 样式文件
resource/static/tests/      # Bucket mock JSON
resource/static/sql/        # SQLite 初始化 SQL
```

## 数据库
```text
./temp/cos.db
```
