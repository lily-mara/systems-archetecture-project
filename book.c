/*
#include<stdlib.h>
#include<string.h>

#include "book.h"

struct book *new_book(char *title)
{
	struct book *book = calloc(sizeof(struct book), 1);
	book->ptr_title = malloc(sizeof(char) * strlen(title) + 1);
	strcpy(book->ptr_title, title);

	return book;
}

struct book *new_book_at_end(char *title, struct book *head)
{
	struct book *book = new_book(title);

	while (head->next != NULL)
	{
		head = head->next;
	}

	head->next = book;

	return book;
}

struct book *new_book_at_beginning(char *title, struct book *head)
{
	struct book *book = new_book(title);
	book->next = head;
	return book;
}
*/
