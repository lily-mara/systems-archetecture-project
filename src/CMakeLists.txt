file(GLOB SOURCES *.c)

set(VALGRIND_TEST_DIR ${PROJECT_SOURCE_DIR}/valgrind_tests)
set(VALGRIND_TEST ${VALGRIND_TEST_DIR}/run_test)

add_executable(my_book_manager ${SOURCES})
add_library(books io.c book.c)

add_test(test_immediate_exit_valgrind ${VALGRIND_TEST}
	${VALGRIND_TEST_DIR}/immediate_exit)

add_test(test_insert_one_valgrind ${VALGRIND_TEST}
	${VALGRIND_TEST_DIR}/insert_one)

add_test(test_insert_five_valgrind ${VALGRIND_TEST}
	${VALGRIND_TEST_DIR}/insert_five)
