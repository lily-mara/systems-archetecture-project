#ifndef _TESTS_H_
#define _TESTS_H_

#include "book.h"

void assert_book_eq(struct book *x, struct book *y);
void assert_list_eq(struct book_node *a, struct book_node *b);
void assert_book_neq(struct book *x, struct book *y);
void assert_pairs_eq(struct book_pair *x, struct book_pair *y);

#endif /* ifndef _TESTS_H_ */
