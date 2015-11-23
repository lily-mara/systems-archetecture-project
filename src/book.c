#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "book.h"

static int count_with_id(struct book_node *head, long id);

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

struct book_node *remove_by_id(struct book_node *head, long id)
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

struct book_node *corrupt_records(struct book_node *head)
{
	struct book_node *current = head, *corrupt = NULL;

	while (current != NULL)
	{
		if (count_with_id(head, current->book->l_book_id))
		{
			append(corrupt, current->book);
		}

		current = current->next;
	}

	return head;
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

void display_corrupt_records(struct book_node *head)
{
	struct book_node *corrupt = corrupt_records(head);
	print_books(corrupt);
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
