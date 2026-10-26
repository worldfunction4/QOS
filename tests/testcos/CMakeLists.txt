cmake_minimum_required(VERSION 3.16)

project(TestCos LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestCos tst_testcos.cpp)
add_test(NAME TestCos COMMAND TestCos)

# 定义宏来设置路径
target_compile_definitions(TestCos PRIVATE TESTCOS_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 头文件项目+SDK
target_include_directories(TestCos PRIVATE
    ${CMAKE_SOURCE_DIR}/qosbrowser
    ${CMAKE_SOURCE_DIR}/qosbrowser/src
    ${COS_SDK_ROOT}/include
    ${COS_SDK_ROOT}/third_party/include
)

# 把lib库所在目录添加上去
target_link_directories(TestCos PRIVATE
    ${COS_SDK_ROOT}/lib
    ${COS_SDK_ROOT}/third_party/lib/x64/poco
    ${COS_SDK_ROOT}/third_party/lib/x64/openssl
)


# 链接的库
target_link_libraries(TestCos PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    cossdk
    PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation
    libssl libcrypto
    ws2_32 iphlpapi crypt32
)

# 需要测的代码以及所依赖的cpp文件
target_sources(TestCos PRIVATE
    ${CMAKE_SOURCE_DIR}/qosbrowser/src/bend/dao/clouds/daocloudscos.cpp
    ${CMAKE_SOURCE_DIR}/qosbrowser/src/config/exceptions.cpp
    ${CMAKE_SOURCE_DIR}/qosbrowser/src/helper/filehelper.cpp
)

# 添加运行时把dll拷贝到相应exe程序
add_custom_command(TARGET TestCos POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${COS_SDK_ROOT}/third_party/lib/x64/poco" "$<TARGET_FILE_DIR:TestCos>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${COS_SDK_ROOT}/third_party/lib/x64/openssl" "$<TARGET_FILE_DIR:TestCos>"
)

include(GNUInstallDirs)
install(TARGETS TestCos
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
