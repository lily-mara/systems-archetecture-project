#ifndef _TESTS_H_
#define _TESTS_H_

void assert_book_eq(struct book *x, struct book *y);
void assert_list_eq(struct book_node *a, struct book_node *b);
void assert_book_neq(struct book *x, struct book *y);

#endif /* ifndef _TESTS_H_ */
