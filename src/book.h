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
void free_node(struct book *);
struct book *remove_by_id(struct book *, long);
struct book *find_by_id(struct book *head, long id);
struct book *find_by_author_id(struct book *first, long id);
struct book *new_book();
void append(struct book **head, struct book *to_append);

#endif /* _BOOK_H_ */
