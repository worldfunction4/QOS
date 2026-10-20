project(qosbrowser LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

qt_add_executable(qosbrowser
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
    src/config/config.h
    resources.qrc
    src/middle/models/bucket.h
    src/middle/models/dbmodels.h

    src/bend/man/mancloud.h src/bend/man/mancloud.cpp
    src/fend/uidelegate/bucketdelegate.h src/fend/uidelegate/bucketdelegate.cpp
    src/bend/man/mandb.h src/bend/man/mandb.cpp
    src/helper/dbsqlite.h src/helper/dbsqlite.cpp
    src/bend/dao/dbs/daologininfosqlite.h src/bend/dao/dbs/daologininfosqlite.cpp
    src/bend/dao/clouds/daoclouds.h
    src/bend/dao/clouds/daocloudsmock.h src/bend/dao/clouds/daocloudsmock.cpp
    src/bend/dao/dbs/daologininfosqlite.h
    src/bend/dao/dbs/daologininfo.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(qosbrowser
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS qosbrowser
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET qosbrowser
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
