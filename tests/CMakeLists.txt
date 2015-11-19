include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable(tests io_tests.c)
add_test(all_tests tests)
target_link_libraries(tests books cmocka)
