#ifndef _IO_H_
#define _IO_H_

#include "book.h"

int export_books(struct book *head, char *filename);
struct book *import_books(char *filename);
long get_long();
int get_int();
float get_float();
char *get_string();

#endif /* _IO_H_ */
