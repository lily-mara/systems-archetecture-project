/**CFile***********************************************************************

  FileName    book.c

  Synopsis    This file contains the book logic for the book manager program.

  SeeAlso     book.h

  Copyright   [Copyright (c) 2015 Carlos III University of Madrid
  All rights reserved.

  Permission is hereby granted, without written agreement and without license
  or royalty fees, to use, copy, modify, and distribute this software and its
  documentation for any purpose, provided that the above copyright notice and
  the following two paragraphs appear in all copies of this software.

  IN NO EVENT SHALL THE CARLOS III UNIVERSITY OF MADRID BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE CARLOS III
  UNIVERSITY OF MADRID HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  THE CARLOS III UNIVERSITY OF MADRID SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN
  "AS IS" BASIS, AND CARLOS III UNIVERSITY OF MADRID HAS NO OBLIGATION TO
  PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.]

******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "book.h"

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/
static int count_with_id(struct book_node *head, long id);
static int author_error(struct book *x, struct book *y);

/*---------------------------------------------------------------------------*/
/* Definition of functions                                                   */
/*---------------------------------------------------------------------------*/
/**Function********************************************************************

  Synopsis           Returns a pointer to the book in the list of books with
                     the given id or NULL if there is no book with the given id

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Frees all of the books nodes within the given list

  SideEffects        Frees all of the books nodes within the given list

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Frees the given list node and the associated book

  SideEffects        Frees the given list node and the associated book

******************************************************************************/
void free_list_node(struct book_node *node)
{
	if (node != NULL)
	{
		free_book(node->book);
		free(node);
	}
}

/**Function********************************************************************

  Synopsis           Removes the first book in the list with the given id, or
                     does nothing if there is no book with the given id.

  SideEffects        May remove a book from the list

******************************************************************************/
struct book_node *remove_first_with_id(struct book_node *head, long id)
{
	struct book *to_remove = find_by_id(head, id);
	if (to_remove != NULL)
	{
		return remove_book(head, to_remove);
	}
	return NULL;
}

/**Function********************************************************************

  Synopsis           Removes the given book from the list of books, or
                     does nothing if the given book is not contained in this
                     list.

  SideEffects        May remove one book from the list

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Frees a book and its associated string pointers.
                     Performs null-checking on the book and strings.

  SideEffects        Frees book and book's pointers.

******************************************************************************/
void free_book(struct book *node)
{
	if (node != NULL)
	{
		if (node->ptr_name != NULL)
		{
			free(node->ptr_name);
		}
		if (node->ptr_surname != NULL)
		{
			free(node->ptr_surname);
		}
		if (node->ptr_title != NULL)
		{
			free(node->ptr_title);
		}
		free(node);
	}
}

/**Function********************************************************************

  Synopsis           Allocates heap space for new book.

  SideEffects        Allocates heap space for new book.

******************************************************************************/
struct book *new_book()
{
	struct book *new = calloc(sizeof(struct book), 1);

	return new;
}

/**Function********************************************************************

  Synopsis           Appends the given book to a list of book nodes. Returns a
                     new head pointer for the list of books. Should be called
                     like: head = append(head, new_book); If the head pointer
                     given is null, this will allocate space and return a fresh
                     head pointer.

  SideEffects        Allocates heap space for new book node.

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Prints corrupted records in the given list of books.

  SideEffects        none other than allocs/frees. Frees all memory that it
                     allocs.

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Gets all pairs of books that have matching author ids but
                     mismatched author names.

  SideEffects        none other than allocs

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Prints corrupted authors from given list of books.

  SeeAlso            corrupt_authors

  SideEffects        none other than allocs/frees. frees all memory it allocs.

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Adds a pair of books to the given linked set of book pairs.
                     If a pairing of (x,y) or (y,x) already exists in this book
                     set, this function does nothing. Returns new head pointer
                     for book set.

  SeeAlso            struct book_pair

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Frees all struct book_pair in this set of book pairs
                     IMPORTANT: does not free books

  SeeAlso            struct book_pair

  SideEffects        frees

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Prints all books in this list of books

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Prints details about the given book

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Get the length of this list of books.

******************************************************************************/
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

/**Function********************************************************************

  Synopsis           Return a list of books that have duplicate ids.

******************************************************************************/
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

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
/**Function********************************************************************

  Synopsis           Returns 1 if the given pairs of books have errors with
                     their authors. A pair of books have an error with their
                     authors if they share the same author id, but either the
                     ptr_name or ptr_surname attributes differ between them.
                     For example, if x and y have author id 33, but
                     x.ptr_name = "Miguel" and y.ptr_name = "Michael", this
                     function would return 1.

******************************************************************************/
static int author_error(struct book *x, struct book *y)
{
		if (x->l_author_id == y->l_author_id &&
				((strcmp(x->ptr_name, y->ptr_name) != 0) ||
				(strcmp(x->ptr_surname, y->ptr_surname) != 0)))
		{
			return 1;
		}
		return 0;
}

/**Function********************************************************************

  Synopsis           Returns the number of books in this list that have the
                     given id.

******************************************************************************/
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
