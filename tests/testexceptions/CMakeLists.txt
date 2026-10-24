cmake_minimum_required(VERSION 3.16)

project(TestExceptions LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestExceptions tst_testexceptions.cpp)
add_test(NAME TestExceptions COMMAND TestExceptions)

target_link_libraries(TestExceptions PRIVATE Qt${QT_VERSION_MAJOR}::Test)
# 添加项目
target_include_directories(TestExceptions PRIVATE ${CMAKE_SOURCE_DIR}/qosbrowser)
# 添加实现
target_sources(TestExceptions PRIVATE
    ${CMAKE_SOURCE_DIR}/qosbrowser/src/config/exceptions.cpp
    ${CMAKE_SOURCE_DIR}/qosbrowser/src/helper/filehelper.cpp)
# 添加路径
target_compile_definitions(TestExceptions PRIVATE
    QOS_DIR="${CMAKE_SOURCE_DIR}/qosbrowser")


include(GNUInstallDirs)
install(TARGETS TestExceptions
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
