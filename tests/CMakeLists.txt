set(CMAKE_AUTORCC ON)   # qt_standard_project_setup 只开了 AUTOMOC/AUTOUIC，.qrc 需要 AUTORCC
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QOS_SRC ${CMAKE_SOURCE_DIR}/qosbrowser)

add_executable(QosTests
    main.cpp
    testfilehelper/tst_testhelper.h      testfilehelper/tst_testhelper.cpp
    testfilehelper/testFile.qrc
    # 主程序的资源，里面有错误码表 errorcode.csv
    ${QOS_SRC}/resources.qrc
    testexceptions/tst_testexceptions.h  testexceptions/tst_testexceptions.cpp
    testcos/tst_testcos.h                testcos/tst_testcos.cpp
    # 测试cos代码
    ${QOS_SRC}/src/helper/filehelper.cpp
    ${QOS_SRC}/src/config/exceptions.cpp
    ${QOS_SRC}/src/bend/dao/clouds/daocloudscos.cpp
)
add_test(NAME QosTests COMMAND QosTests)

target_compile_definitions(QosTests PRIVATE
    QOS_DIR="${QOS_SRC}"
    TESTCOS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/testcos"
)

target_include_directories(QosTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QOS_SRC}
    ${QOS_SRC}/src
    ${COS_SDK_ROOT}/include
    ${COS_SDK_ROOT}/third_party/include
)

target_link_directories(QosTests PRIVATE
    ${COS_SDK_ROOT}/lib
    ${COS_SDK_ROOT}/third_party/lib/x64/poco
    ${COS_SDK_ROOT}/third_party/lib/x64/openssl
)

target_link_libraries(QosTests PRIVATE
    Qt6::Test
    cossdk
    PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation
    libssl libcrypto
    ws2_32 iphlpapi crypt32
)

# QosTests和qosbrowser.exe输出在同一个目录，dll由qosbrowser的POST_BUILD拷一次就够
# 两个目标并行拷同一批dll会抢文件导致构建失败，所以让测试等qosbrowser构建完
add_dependencies(QosTests qosbrowser)