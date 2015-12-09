/**CFile***********************************************************************

  FileName    io.c

  Synopsis    This file contains the IO logic for the book manager program. The
              first section contains the functions used for importing and
              exporting the book list to files on the filesystem.

              The first bytes of the file are a size_t corresponding to the
              number of elements stored in the file.

              The books are stored in the order that they are given in the list,
              with the fields being in the following order:

              book_id (long)
              author_id (long)
              year (int)
              num_pages (int)
              quality (float)
              number of characters in title string (size_t)
              number of characters in name string (size_t)
              number of characters in surname string (size_t)
              title (char *)
              name (char *)
              surname (char *)

              The next functions deal with reading data of various types from
              stdin.

  SeeAlso     io.h

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "io.h"
#include "book.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/
#define ERROR_INVALID_NUMBER "\tInvalid. Expected number, got '%s'. Please input number: "

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/
/**Macro***********************************************************************

  Synopsis     returns from the parent function if the two arguments are not
               equal

  SideEffects  may return from function

******************************************************************************/
#define _early_return_ne(x,y) if((x) != (y)){return -1;}

/**Macro***********************************************************************

  Synopsis     returns from the parent function if the argument is null

  SideEffects  may return from function

******************************************************************************/
#define _early_return_null(x) if((x) == NULL){return -1;}

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/
static struct book_node *load_books_from_fp(FILE *);
static int save_books_to_fp(struct book_node *, FILE *);
static struct book *load_book_from_fp(FILE *fp);

/*---------------------------------------------------------------------------*/
/* Definition of functions                                                   */
/*---------------------------------------------------------------------------*/
/**Function********************************************************************

  Synopsis           Saves the list of books given by the head pointer to the
                     file given by the filename. Returns -1 if the write failed,
                     0 otherwise.

  SideEffects        Disk IO

******************************************************************************/
int export_books(struct book_node *head, char *filename)
{
	FILE *fp = fopen(filename, "w");
	_early_return_null(fp);

	int status = save_books_to_fp(head, fp);
	fclose(fp);

	return status;
}

/**Function********************************************************************

  Synopsis           Reads a list of books from the file given by filename.
                     Returns NULL if there is a problem with the IO.

  SideEffects        Disk IO

******************************************************************************/
struct book_node *import_books(char *filename)
{
	struct book_node *head;
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return NULL;
	}

	head = load_books_from_fp(fp);
	fclose(fp);

	return head;
}

/**Function********************************************************************

  Synopsis           Reads a long int from stdin, continues to ask the user to
                     enter a number if an invalid number literal is entered.
                     Writes the long to the given pointer and returns a status
                     code for the read operation. If the user enters CTRL-D at
                     this prompt, returns GET_USR_ENTERED_CTRLD otherwise
                     returns GET_SUCCESS.

  SideEffects        Read from stdin

******************************************************************************/
int get_long(long *x)
{
	char *aux = get_string();
	char *aux2;
	long out;

	if (aux == NULL)
	{
		return GET_USR_ENTERED_CTRLD;
	}

	out = strtol(aux, &aux2, 10);

	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_long(x);
	}
	else
	{
		free(aux);
	}

	*x = out;
	return GET_SUCCESS;
}

/**Function********************************************************************

  Synopsis           Reads a int from stdin, continues to ask the user to
                     enter a number if an invalid number literal is entered.
                     Writes the int to the given pointer and returns a status
                     code for the read operation. If the user enters CTRL-D at
                     this prompt, returns GET_USR_ENTERED_CTRLD otherwise
                     returns GET_SUCCESS.

  SideEffects        Read from stdin

******************************************************************************/
int get_int(int *x)
{
	char *aux = get_string();
	int out;
	char *aux2;

	if (aux == NULL)
	{
		return GET_USR_ENTERED_CTRLD;
	}

	out = strtol(aux, &aux2, 10);


	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_int(x);
	}
	else
	{
		free(aux);
	}

	*x = out;
	return GET_SUCCESS;
}

/**Function********************************************************************

  Synopsis           Reads a float from stdin, continues to ask the user to
                     enter a number if an invalid number literal is entered.
                     Writes the float to the given pointer and returns a status
                     code for the read operation. If the user enters CTRL-D at
                     this prompt, returns GET_USR_ENTERED_CTRLD otherwise
                     returns GET_SUCCESS.

  SideEffects        Read from stdin

******************************************************************************/
int get_float(float *x)
{
	char *aux = get_string();
	char *aux2;
	float out;

	if (aux == NULL)
	{
		return GET_USR_ENTERED_CTRLD;
	}

	out = strtof(aux, &aux2);

	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_float(x);
	}
	else
	{
		free(aux);
	}

	*x = out;
	return GET_SUCCESS;
}

/**Function********************************************************************

  Synopsis           Reads a string from stdin, continues to ask the user to
                     enter a string if the user enters an empty string. If the
                     user hits CTRL-D, then this function will return NULL.

  SideEffects        Read from stdin

******************************************************************************/
char *get_string()
{
	size_t size = 0;
	char *ptr = NULL;
	size = getline(&ptr, &size, stdin);

	if ((int)size == -1)
	{
		printf("\n");
		return NULL;
	}
	else
	{
		ptr[size-1] = '\0';
	}

	if ((int)size == -1 || strlen(ptr) <= 0)
	{
		free(ptr);
		printf("You may not input an empty string: ");
		return get_string();
	}

	return ptr;
}

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/
/**Function********************************************************************

  Synopsis           Reads a list of books from the file given by fp.
                     Returns NULL if there is a problem with the IO.

  SideEffects        Disk IO

******************************************************************************/
static struct book_node *load_books_from_fp(FILE *fp)
{
	struct book_node *head, *current;
	size_t elements, i;

	if (feof(fp))
	{
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	i = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	if (i == 0)
	{
		return NULL;
	}

	i = fread(&elements, sizeof(size_t), 1, fp);
	/*assert(i */

	head = append(NULL, load_book_from_fp(fp));
	current = head;

	for (i = 1; i < elements; i++)
	{
		append(current, load_book_from_fp(fp));
		current = current->next;
	}

	return head;
}

/**Function********************************************************************

  Synopsis           Writes a list of books to the file given by fp.
                     Returns NULL if there is a problem with the IO.

  SideEffects        Disk IO

******************************************************************************/
static int save_books_to_fp(struct book_node *head, FILE *fp)
{
	size_t elements_written, title_chars, name_chars, surname_chars, length;
	struct book *current;

	length = book_list_length(head);
	elements_written = fwrite(&length, sizeof(size_t), 1, fp);
	_early_return_ne(elements_written, 1);

	while (head != NULL) {
		current = head->book;
		elements_written = fwrite(&(current->l_book_id), sizeof(long), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(current->l_author_id), sizeof(long), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(current->i_year), sizeof(int), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(current->i_numb_pages), sizeof(int), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(current->f_quality), sizeof(float), 1, fp);
		_early_return_ne(elements_written, 1);

		title_chars = strlen(current->ptr_title);
		elements_written = fwrite(&title_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		name_chars = strlen(current->ptr_name);
		elements_written = fwrite(&name_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		surname_chars = strlen(current->ptr_surname);
		elements_written = fwrite(&surname_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(current->ptr_title, sizeof(char), title_chars, fp);
		_early_return_ne(elements_written, title_chars);

		elements_written = fwrite(current->ptr_name, sizeof(char), name_chars, fp);
		_early_return_ne(elements_written, name_chars);

		elements_written = fwrite(current->ptr_surname, sizeof(char), surname_chars, fp);
		_early_return_ne(elements_written, surname_chars);

		head = head->next;
	}

	return 0;
}

/**Function********************************************************************

  Synopsis           Reads a single book from the file given by filename.
                     Returns NULL if there is a problem with the IO, or the eof
                     has been reached

  SideEffects        Disk IO

******************************************************************************/
static struct book *load_book_from_fp(FILE *fp)
{
	size_t bytes_read, title_chars, name_chars, surname_chars;
	struct book *b;
	if (feof(fp))
	{
		return NULL;
	}

	b = new_book();

	bytes_read = fread(&b->l_book_id, sizeof(long), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&b->l_author_id, sizeof(long), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&b->i_year, sizeof(int), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&b->i_numb_pages, sizeof(int), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&b->f_quality, sizeof(float), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&title_chars, sizeof(size_t), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&name_chars, sizeof(size_t), 1, fp);
	assert(bytes_read == 1);

	bytes_read = fread(&surname_chars, sizeof(size_t), 1, fp);
	assert(bytes_read == 1);

	b->ptr_title = malloc(title_chars * sizeof(char) + 1);
	b->ptr_name = malloc(name_chars * sizeof(char) + 1);
	b->ptr_surname = malloc(surname_chars * sizeof(char) + 1);

	b->ptr_title[title_chars] = 0;
	b->ptr_name[name_chars] = 0;
	b->ptr_surname[surname_chars] = 0;

	bytes_read = fread(b->ptr_title, sizeof(char), title_chars, fp);
	assert(bytes_read == title_chars);

	bytes_read = fread(b->ptr_name, sizeof(char), name_chars, fp);
	assert(bytes_read == name_chars);

	bytes_read = fread(b->ptr_surname, sizeof(char), surname_chars, fp);
	assert(bytes_read == surname_chars);

#ifndef DEBUG
	(void)sizeof(bytes_read);
#endif /* DEBUG */

	return b;
}
