cmake_minimum_required(VERSION 3.19)
project(MainWindow LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

if(MSVC)
    add_compile_options(/utf-8)  # 强制MSVC编译器将源文件和执行字符集都视为 UTF-8
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fexec-charset=UTF-8 -finput-charset=UTF-8)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(MainWindow
    WIN32 MACOSX_BUNDLE
    main.cpp

    src/fend/uimain/uimain.h src/fend/uimain/uimain.cpp src/fend/uimain/uimain.ui
    src/fend/uimain/bucketswidget.h src/fend/uimain/bucketswidget.cpp src/fend/uimain/bucketswidget.ui
    src/fend/uimain/toolbarwidget.h src/fend/uimain/toolbarwidget.cpp src/fend/uimain/toolbarwidget.ui
    src/fend/uicom/breadwidget.h src/fend/uicom/breadwidget.cpp src/fend/uicom/breadwidget.ui
    src/fend/uimain/objectwidget.h src/fend/uimain/objectwidget.cpp src/fend/uimain/objectwidget.ui
    src/fend/uicom/pagewidget.h src/fend/uicom/pagewidget.cpp src/fend/uicom/pagewidget.ui
    src/fend/uilogin/loginwidget.h src/fend/uilogin/loginwidget.cpp src/fend/uilogin/loginwidget.ui
    src/helper/filehelper.h src/helper/filehelper.cpp
    resources.qrc
    src/bend/models/bucket.h src/bend/models/bucket.cpp
    src/bend/dao/daobuckets.h src/bend/dao/daobuckets.cpp
    src/man/manbucket.h src/man/manbucket.cpp
    src/fend/uidelegate/bucketdelegate.h src/fend/uidelegate/bucketdelegate.cpp
    src/man/mandb.h src/man/mandb.cpp
    src/helper/dbsqlite.h src/helper/dbsqlite.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MainWindow
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS MainWindow
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MainWindow
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
