#ifndef _BOOK_H_
#define _BOOK_H_

#include <stdlib.h>

struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	int i_numb_pages;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
};

struct book_node
{
	struct book *book;
	struct book_node *next;
};

struct book_pair
{
	struct book *x;
	struct book *y;
	struct book_pair *next;
};


void free_list(struct book_node *);
void free_list_node(struct book_node *);
void free_book(struct book *);
struct book_node *remove_all_with_id(struct book_node *, long);
struct book_node *remove_first_with_id(struct book_node *, long);
struct book *find_by_id(struct book_node *head_node, long id);
struct book *find_by_author_id(struct book_node *first, long id);
struct book *new_book();
struct book_node *append(struct book_node *, struct book *);
void display_corrupt_records(struct book_node *);
void print_books(struct book_node *);
void print_book(struct book *);
size_t book_list_length(struct book_node *);
struct book_node *remove_book(struct book_node *, struct book *);
struct book_node *all_with_id(struct book_node *head, long id);
struct book_node *duplicate_ids(struct book_node *head);
struct book_node *duplicate(struct book_node *head);
struct book_pair *corrupt_authors(struct book_node *head);
void print_corrupt_authors(struct book_node *head);
void free_pairs(struct book_pair *head);
struct book_pair *add_pair(struct book_pair *head, struct book *x, struct book *y);

#endif /* _BOOK_H_ */
