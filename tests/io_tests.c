#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmocka.h>
#include <string.h>

#include "book.h"
#include "io.h"
#include "tests.h"

void free_list_nodes(struct book_node *head)
{
	struct book_node *tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void test_equality(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_eq((&x), (&y));
}

void test_not_eq_id(void **state) {
	(void) state;
	struct book x = {20003,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_title(void **state) {
	(void) state;
	struct book x = {20002,"Don Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_year(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1606,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_pages(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1377,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_quality(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1376,101.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_author_id(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1376,100.0,10002,"Miguel","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_author_name(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1376,100.0,10001,"Tomas","de Cervantes Saavedra"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_not_eq_author_surname(void **state) {
	(void) state;
	struct book x = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","Woodhall"};
	struct book y = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};

	assert_book_neq((&x), (&y));
}

void test_write_one_to_file_and_retrive(void **state) {
	(void) state;
	struct book in = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node node_in = {&in, NULL};
	struct book_node *out;

	int status = export_books(&node_in, "__test_write_one_to_file_and_retrive.test.dat");
	assert_true(status == 0);

	out = import_books("__test_write_one_to_file_and_retrive.test.dat");

	assert_book_eq(&in, out->book);
	free_list(out);
}

void test_write_two_to_file_and_retrive(void **state) {
	(void) state;
	struct book two = {20003,"The Da Vinci Code",1990,500,100.0,10002,"Dan","Brown"};
	struct book one = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node *in = append(NULL, &one);
	in = append(in, &two);

	struct book_node *out;

	int status = export_books(in, "__test_write_two_to_file_and_retrive.test.dat");
	assert_true(status == 0);

	out = import_books("__test_write_two_to_file_and_retrive.test.dat");

	assert_list_eq(in, out);
	free_list(out);
	free_list_nodes(in);
}

void test_write_three_to_file_and_retrive(void **state) {
	(void) state;
	struct book one = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book two = {20003,"The Da Vinci Code",1990,500,100.0,10002,"Dan","Brown"};
	struct book three = {20004,"The C Programming Language",1982,501,99.0,10003,"Ken","Thomson"};
	struct book_node *out, *in = append(NULL, &one);
	in = append(in, &two);
	in = append(in, &three);

	int status = export_books(in, "__test_write_three_to_file_and_retrive.test.dat");
	assert_true(status == 0);

	out = import_books("__test_write_three_to_file_and_retrive.test.dat");

	assert_list_eq(in, out);
	free_list_nodes(in);
	free_list(out);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_equality),
		cmocka_unit_test(test_not_eq_id),
		cmocka_unit_test(test_not_eq_title),
		cmocka_unit_test(test_not_eq_year),
		cmocka_unit_test(test_not_eq_pages),
		cmocka_unit_test(test_not_eq_quality),
		cmocka_unit_test(test_not_eq_author_id),
		cmocka_unit_test(test_not_eq_author_name),
		cmocka_unit_test(test_not_eq_author_surname),
		cmocka_unit_test(test_write_one_to_file_and_retrive),
		cmocka_unit_test(test_write_two_to_file_and_retrive),
		cmocka_unit_test(test_write_three_to_file_and_retrive),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
