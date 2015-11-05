/*
#ifndef _BOOK_H_
#define _BOOK_H_

struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
	struct book *next;
};

struct book *new_book(char *title);
struct book *new_book_at_end(char *title, struct book *head);
struct book *new_book_at_beginning(char *title, struct book *head);

#endif /* _BOOK_H_ */
*/
