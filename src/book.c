#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "book.h"

static int count_with_id(struct book_node *head, long id);
static int author_error(struct book *x, struct book *y);

struct book *find_by_id(struct book_node *head, long id)
{
	while (head != NULL)
	{
		if (head->book->l_book_id == id)
		{
			return head->book;
		}
		head = head->next;
	}

	return NULL;
}

struct book *find_by_author_id(struct book_node *head, long id)
{
	while (head != NULL)
	{
		if (head->book->l_author_id == id)
		{
			return head->book;
		}
		head = head->next;
	}

	return NULL;
}

void free_list(struct book_node *head)
{
	struct book_node *temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;

		free_list_node(temp);
	}
}

void free_list_node(struct book_node *node)
{
	free_book(node->book);
	free(node);
}


struct book_node *remove_first_with_id(struct book_node *head, long id)
{
	return remove_book(head, find_by_id(head, id));
}

struct book_node *remove_all_with_id(struct book_node *head, long id)
{
	struct book_node *current = head, *temp = head, *prev = head;

	if (head->book->l_book_id == id)
	{
		temp = head->next;
		prev = temp;
		free_list_node(head);

		head = temp;
		current = temp;
	}

	while (current != NULL)
	{
		prev = temp;
		temp = head;
		current = current->next;

		if (temp->book->l_book_id == id)
		{
			free_list_node(temp);

			prev->next = current;
		}
	}

	return head;
}

struct book_node *remove_book(struct book_node *head, struct book *to_remove)
{
	struct book_node *current = head, *temp = head, *prev = head;

	if (head->book == to_remove)
	{
		temp = head->next;
		free_list_node(head);

		return temp;
	}

	while (current != NULL)
	{
		prev = temp;
		temp = head;
		current = current->next;

		if (temp->book == to_remove)
		{
			free_list_node(temp);

			prev->next = current;
			return head;
		}
	}

	return head;
}

void free_book(struct book *node)
{
	free(node->ptr_name);
	free(node->ptr_title);
	free(node->ptr_surname);
	free(node);
}


struct book *new_book()
{
	struct book *new = calloc(sizeof(struct book), 1);

	return new;
}

struct book_node *append(struct book_node *head, struct book *new)
{
	struct book_node *tmp = head, *end = malloc(sizeof(struct book_node));
	end->next = NULL;
	end->book = new;

	if (head == NULL)
	{
		return end;
	}

	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}

	tmp->next = end;
	return head;
}

void display_corrupt_records(struct book_node *head)
{
	struct book_node *corrupt = duplicate_ids(head), *temp;
	struct book *b;
	if (corrupt != NULL)
	{
		printf("\nBooks with duplicate ids:\n");
	}
	while (corrupt != NULL)
	{
		b = corrupt->book;
		printf("%ld => %s\n", b->l_book_id, b->ptr_title);
		temp = corrupt;
		corrupt = corrupt->next;

		free(temp);
	}

	free_list(corrupt);
	printf("\n");

	print_corrupt_authors(head);
}

struct book_pair *corrupt_authors(struct book_node *head)
{
	struct book_pair *corrupt = NULL;
	struct book_node *i = head, *j;

	while (i != NULL)
	{
		j = head;
		while (j != NULL)
		{
			if (author_error(i->book, j->book))
			{
				corrupt = add_pair(corrupt, i->book, j->book);
			}
			j = j->next;
		}
		i = i->next;
	}

	return corrupt;
}

void print_corrupt_authors(struct book_node *head)
{
	struct book_pair *tmp, *corrupt = corrupt_authors(head);

	if (corrupt != NULL)
	{
		printf("CORRUPT AUTHORS:\n");
	}

	while (corrupt != NULL)
	{
		printf("Book %ld and %ld: \"%s %s\" != \"%s %s\"\n",
				corrupt->x->l_book_id, corrupt->y->l_book_id,
				corrupt->x->ptr_name, corrupt->x->ptr_surname,
				corrupt->y->ptr_name, corrupt->y->ptr_surname);
		tmp = corrupt;
		corrupt = corrupt->next;
		free(tmp);
	}

	printf("\n");
}

struct book_pair *add_pair(struct book_pair *head, struct book *x, struct book *y)
{
	struct book_pair *tmp = head, *prev = head;

	if (head == NULL)
	{
		tmp = malloc(sizeof(struct book_pair));
		tmp->x = x;
		tmp->y = y;
		tmp->next = NULL;

		return tmp;
	}

	while (tmp != NULL)
	{
		if ((tmp->x == x && tmp->y == y) || (tmp->x == y && tmp->y == x))
		{
			return head;
		}
		prev = tmp;
		tmp = tmp->next;
	}

	prev->next = malloc(sizeof(struct book_pair));
	prev->next->x = x;
	prev->next->y = y;
	prev->next->next = NULL;

	return head;
}

void free_pairs(struct book_pair *head)
{
	struct book_pair *tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

static int author_error(struct book *x, struct book *y)
{
		if (x->l_author_id == y->l_author_id && ((strcmp(x->ptr_name, y->ptr_name) != 0) || (strcmp(x->ptr_surname, y->ptr_surname) != 0)))
		{
			return 1;
		}
		return 0;
}

void print_books(struct book_node *head)
{
	struct book *b;
	printf("\n===============================================================");
	printf("======================================\n== ""Book id"" |""  Títle (20 chars )""| ""Author id"" ==========================================================");
	printf("\n===============================================================");
	printf("======================================");
	printf("\n===============================================================");
	printf("======================================\n");

	while (head != NULL)
	{
		b = head->book;
		printf("== %-ld |%-20s|%-ld\n", b->l_book_id, b->ptr_title, b->l_author_id);
		head = head->next;
	}
	printf("\n=====================================================");
	printf("================================================\n");
}

void print_book(struct book *b)
{
	printf("Detailed information for==(%ld)==\n", b->l_book_id);
	printf("=%ld= %31s %ld\n", b->l_book_id, "Identifier:", b->l_book_id);
	printf("=%ld= %31s %s\n", b->l_book_id, "Title of the book:", b->ptr_title);
	printf("=%ld= %31s %i\n", b->l_book_id, "Publication year:", b->i_year);
	printf("=%ld= %31s %i\n", b->l_book_id, "Number of pages:", b->i_numb_pages);
	printf("=%ld= %31s %lf\n", b->l_book_id, "Quality:", b->f_quality);
	printf("=%ld= %31s %ld\n", b->l_book_id, "Authors' ID:", b->l_author_id);
	printf("=%ld= %31s %s\n", b->l_book_id, "Name of the author:", b->ptr_name);
	printf("=%ld= %31s %s\n", b->l_book_id, "Surname of the authour:", b->ptr_surname);
}

size_t book_list_length(struct book_node *head)
{
	size_t length = 0;
	while (head != NULL)
	{
		length += 1;
		head = head->next;
	}

	return length;
}

struct book_node *all_with_id(struct book_node *head, long id)
{
	struct book_node *new_list = NULL;

	while (head != NULL)
	{
		if (head->book->l_book_id == id)
		{
			new_list = append(new_list, head->book);
		}
		head = head->next;
	}

	return new_list;
}

struct book_node *duplicate(struct book_node *head)
{
	struct book_node *new_head = NULL;

	while (head != NULL)
	{
		new_head = append(new_head, head->book);
		head = head->next;
	}

	return new_head;
}

struct book_node *duplicate_ids(struct book_node *head)
{
	struct book_node *dups = NULL, *tmp = head;

	while (tmp != NULL)
	{
		if (count_with_id(head, tmp->book->l_book_id) > 1)
		{
			dups = append(dups, tmp->book);
		}
		tmp = tmp->next;
	}

	return dups;
}

static int count_with_id(struct book_node *head, long id)
{
	int count = 0;

	while (head != NULL)
	{
		if (head->book->l_book_id == id)
		{
			count += 1;
		}
		head = head->next;
	}

	return count;
}

struct book_node *mismatched_authors(struct book_node *head)
{
	struct book_node *mismatched = NULL, *temp = NULL, *to_free;

	while (head != NULL)
	{
		temp = all_with_id(head->next, head->book->l_book_id);
		if (temp != NULL)
		{
			mismatched = append(mismatched, head->book);
		}

		while (temp != NULL)
		{
			mismatched = append(mismatched, temp->book);
			to_free = temp;
			temp = temp->next;
			free(to_free);
		}
		head = head->next;
	}

	return mismatched;
}
