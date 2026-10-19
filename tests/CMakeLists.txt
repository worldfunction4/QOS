qt_add_executable(tests tst_qos.cpp)
target_link_libraries(tests PRIVATE Qt::Test)
add_test(NAME tests COMMAND tests)