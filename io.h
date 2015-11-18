#ifndef _IO_H_
#define _IO_H_

#include "my_book_manager.h"

int save_books_to_filename(struct book *head, char *filename);
struct book *load_books_from_filename(char *filename);

#endif /* _IO_H_ */
