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
};

struct book_node
{
	struct book *book;
	struct book_node *next;
};

void free_list(struct book_node *);
void free_list_node(struct book_node *);
void free_book(struct book *);
struct book_node *remove_by_id(struct book_node *, long);
struct book *find_by_id(struct book_node *head_node, long id);
struct book *find_by_author_id(struct book_node *first, long id);
struct book *new_book();
struct book_node *append(struct book_node *, struct book *);
struct book_node *corrupt_records(struct book_node *head);
void display_corrupt_records(struct book_node *);
void print_books(struct book_node *);
void print_book(struct book *);
size_t book_list_length(struct book_node *);

#endif /* _BOOK_H_ */
