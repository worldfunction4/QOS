cmake_minimum_required(VERSION 3.16)

project(TestHelper LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestHelper tst_testhelper.cpp
    testFile.qrc)
add_test(NAME TestHelper COMMAND TestHelper)

target_link_libraries(TestHelper PRIVATE Qt${QT_VERSION_MAJOR}::Test)
# 添加需要测试的头文件
target_include_directories(TestHelper PRIVATE ${CMAKE_SOURCE_DIR}/qosbrowser)
# 添加实现
target_sources(TestHelper PRIVATE ${CMAKE_SOURCE_DIR}/qosbrowser/src/helper/filehelper.cpp)

include(GNUInstallDirs)
install(TARGETS TestHelper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
