#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdlib.h>

#include "book.h"
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

void test_two_duplicate_ids(void **state) {
	(void) state;
	struct book a = {20003,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20001,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book d = {20004,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book e = {20005,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book f = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node *out, *in = append(NULL, &a), *expected = append(NULL, &b);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);
	expected = append(expected, &f);

	out = duplicate_ids(in);

	assert_list_eq(out, expected);

	free_list_nodes(in);
	free_list_nodes(expected);
	free_list_nodes(out);
}

void test_four_duplicate_ids(void **state) {
	(void) state;
	struct book a = {20003,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20001,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book d = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book e = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book f = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node *out, *in = append(NULL, &a), *expected = append(NULL, &b);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);
	expected = append(expected, &d);
	expected = append(expected, &e);
	expected = append(expected, &f);

	out = duplicate_ids(in);

	assert_list_eq(out, expected);

	free_list_nodes(in);
	free_list_nodes(expected);
	free_list_nodes(out);
}

void test_no_duplicates(void **state) {
	(void) state;
	struct book a = {20001,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20003,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book d = {20004,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book e = {20005,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book f = {20006,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node *out, *in = append(NULL, &a);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);

	out = duplicate_ids(in);

	assert_ptr_equal(out, NULL);

	free_list_nodes(in);
	free_list_nodes(out);
}

void test_no_author_problems(void **state) {
	(void) state;
	struct book a = {20001,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20003,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book d = {20004,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book e = {20005,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book f = {20006,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book_node *in = append(NULL, &a);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);

	struct book_pair *out = corrupt_authors(in);

	assert_ptr_equal(out, NULL);

	free_list_nodes(in);
	free_pairs(out);
}

void test_two_author_errors(void **state) {
	(void) state;
	struct book a = {20001,"El Quijote",1605,1376,100.0,10003,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20003,"El Quijote",1605,1376,100.0,10003,"Micheal","de Cervantes Saavedra"};
	struct book d = {20004,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book e = {20005,"El Quijote",1605,1376,100.0,10002,"Miguel","de"};
	struct book f = {20006,"El Quijote",1605,1376,100.0,10002,"Miguel","de Cervantes Saavedra"};
	struct book_node *in = append(NULL, &a);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);

	struct book_pair *expected = NULL, *out = corrupt_authors(in);
	expected = add_pair(expected, &a, &c);
	expected = add_pair(expected, &e, &f);

	assert_pairs_eq(expected, out);

	free_list_nodes(in);
	free_pairs(out);
	free_pairs(expected);
}

void test_overlapping_author_errors(void **state) {
	(void) state;
	struct book a = {20001,"El Quijote",1605,1376,100.0,10000,"Miguel","de Cervantes Saavedra"};
	struct book b = {20002,"El Quijote",1605,1376,100.0,10001,"Miguel","de Cervantes Saavedra"};
	struct book c = {20003,"El Quijote",1605,1376,100.0,10001,"Micheal","de Cervantes Saavedra"};
	struct book d = {20004,"El Quijote",1605,1376,100.0,10000,"Miguel","de Cervantes Saavedra"};
	struct book e = {20005,"El Quijote",1605,1376,100.0,10001,"Miguel","de"};
	struct book f = {20006,"El Quijote",1605,1376,100.0,10000,"Miguel","de Cervantes Saavedra"};
	struct book_node *in = append(NULL, &a);
	in = append(in, &b);
	in = append(in, &c);
	in = append(in, &d);
	in = append(in, &e);
	in = append(in, &f);

	struct book_pair *expected = NULL, *out = corrupt_authors(in);
	expected = add_pair(expected, &b, &c);
	expected = add_pair(expected, &b, &e);
	expected = add_pair(expected, &c, &e);

	assert_pairs_eq(expected, out);

	free_list_nodes(in);
	free_pairs(out);
	free_pairs(expected);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_two_duplicate_ids),
		cmocka_unit_test(test_no_duplicates),
		cmocka_unit_test(test_four_duplicate_ids),
		cmocka_unit_test(test_no_author_problems),
		cmocka_unit_test(test_two_author_errors),
		cmocka_unit_test(test_overlapping_author_errors),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
