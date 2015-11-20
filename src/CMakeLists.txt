file(GLOB SOURCES *.c)

set(VALGRIND_TEST_DIR ${PROJECT_SOURCE_DIR}/valgrind_tests)
set(VALGRIND_TEST ${VALGRIND_TEST_DIR}/run_test)

function(add_valgrind_test filename)
	foreach(loop_var ${ARGN})
		list(APPEND other_args "${VALGRIND_TEST_DIR}/${loop_var}")
	endforeach(loop_var)

	list(LENGTH ARGN other_args_length)
	if(other_args_length STREQUAL 0)
		set(test_name ${filename})
	else(other_args_length STREQUAL 0)
		list (GET ARGN 0 HEAD)
		set(test_name ${HEAD})
	endif(other_args_length STREQUAL 0)

	add_test(test_${test_name}_valgrind ${VALGRIND_TEST}
		${VALGRIND_TEST_DIR}/${filename} ${other_args} ${VALGRIND_TEST_DIR}/exit)
endfunction(add_valgrind_test)

add_executable(my_book_manager ${SOURCES})
add_library(books io.c book.c)

add_valgrind_test(exit)
add_valgrind_test(insert_one)
add_valgrind_test(insert_one insert_one_save)
add_valgrind_test(insert_one insert_one_save_load)
add_valgrind_test(insert_five)
add_valgrind_test(insert_five insert_five_save)
add_valgrind_test(insert_five insert_five_save_load)
add_valgrind_test(lots_of_wrong_numbers)
add_valgrind_test(non_numbers)
