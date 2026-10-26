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
    src/config/global.h
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
    src/plugins/manplugin.h src/plugins/manplugin.cpp
    src/bend/dao/config/version.h
    src/bend/dao/config/versionjson.h src/bend/dao/config/versionjson.cpp
    src/bend/dao/config/version.cpp
    src/bend/dao/config/versioncmd.h src/bend/dao/config/versioncmd.cpp
    src/bend/dao/logs/basiclogger.h src/bend/dao/logs/basiclogger.cpp
    src/bend/dao/logs/loggerdebug.h src/bend/dao/logs/loggerdebug.cpp
    src/config/loggerproxy.h src/config/loggerproxy.cpp
    src/config/apis.h
    src/config/errorcode.h
    src/config/exceptions.h src/config/exceptions.cpp
    src/middle/signals/mansignals.h src/middle/signals/mansignals.cpp
    src/bend/gateway.h src/bend/gateway.cpp
    src/middle/manglobal.h
    src/middle/manglobal.cpp
    src/bend/dao/clouds/daocloudscos.h src/bend/dao/clouds/daocloudscos.cpp
    src/middle/models/cloudobject.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${COS_SDK_ROOT}/include
    ${COS_SDK_ROOT}/third_party/include
)
target_link_directories(qosbrowser PRIVATE
    ${COS_SDK_ROOT}/lib
    ${COS_SDK_ROOT}/third_party/lib/x64/poco
    ${COS_SDK_ROOT}/third_party/lib/x64/openssl
)

target_link_libraries(qosbrowser
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::Concurrent
        # cos SDK
        cossdk
        # poco
        PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation
        # openSSL
        libssl libcrypto
        # win系统库
        ws2_32 iphlpapi crypt32
)


# copy runtime DLLs next to the exe
add_custom_command(TARGET qosbrowser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${COS_SDK_ROOT}/third_party/lib/x64/poco" "$<TARGET_FILE_DIR:qosbrowser>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${COS_SDK_ROOT}/third_party/lib/x64/openssl" "$<TARGET_FILE_DIR:qosbrowser>"
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
