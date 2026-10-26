cmake_minimum_required(VERSION 3.19)
project(qos LANGUAGES CXX)

if(MSVC)
    add_compile_options(/utf-8)  # 强制MSVC编译器将源文件和执行字符集都视为 UTF-8
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fexec-charset=UTF-8 -finput-charset=UTF-8)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql Test Concurrent)
qt_standard_project_setup()
enable_testing()

# 添加SDK
set(COS_SDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/qosbrowser/third/cos)

add_subdirectory(qosbrowser)

# 添加测试文件夹
add_subdirectory(tests)