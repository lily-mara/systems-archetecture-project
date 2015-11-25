#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdarg.h>
#include <cmocka.h>

#include "book.h"

void assert_book_eq(struct book *x, struct book *y) {
	assert_ptr_not_equal(x, NULL);
	assert_ptr_not_equal(y, NULL);
	assert_true(x->l_book_id == y->l_book_id);
	assert_true(x->l_author_id == y->l_author_id);
	assert_true(x->i_year == y->i_year);
	assert_true(x->i_numb_pages == y->i_numb_pages);
	assert_true(x->f_quality == y->f_quality);
	assert_string_equal(x->ptr_title, y->ptr_title);
	assert_string_equal(x->ptr_name, y->ptr_name);
	assert_string_equal(x->ptr_surname, y->ptr_surname);
}

void assert_list_eq(struct book_node *a, struct book_node *b) {
	assert_int_equal(book_list_length(a), book_list_length(b));

	while (a != NULL || b != NULL) {
		assert_book_eq(a->book, b->book);

		a = a->next;
		b = b->next;
	}
}

void assert_book_neq(struct book *x, struct book *y) {
	assert_true(
		x->l_book_id != y->l_book_id ||
		x->l_author_id != y->l_author_id ||
		strcmp(x->ptr_title, y->ptr_title) != 0 ||
		strcmp(x->ptr_name, y->ptr_name) != 0 ||
		strcmp(x->ptr_surname, y->ptr_surname) != 0 ||
		x->i_year != y->i_year ||
		x->i_numb_pages != y->i_numb_pages ||
		x->f_quality != y->f_quality
	);
}
