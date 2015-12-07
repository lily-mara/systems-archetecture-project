#ifndef _IO_H_
#define _IO_H_

#define GET_USR_ENTERED_CTRLD 1
#define GET_SUCCESS 0

#include "book.h"

int export_books(struct book_node *head, char *filename);
struct book_node *import_books(char *filename);
int get_long(long *x);
int get_int(int *x);
int get_float(float *x);
char *get_string();

#endif /* _IO_H_ */
