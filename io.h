#ifndef _IO_H_
#define _IO_H_

#include "book.h"

int export_books(struct book *head, char *filename);
struct book *import_books(char *filename);

#endif /* _IO_H_ */
