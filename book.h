#ifndef _BOOK_H_
#define _BOOK_H_

struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	int i_numb_pages;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
	struct book *next;
};

void free_list(struct book *);
struct book *remove_by_id(struct book *, int);

#endif /* _BOOK_H_ */
