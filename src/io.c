#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "io.h"
#include "book.h"

/*
 * This file contains the IO logic for the book manager program. The first
 * section contains the functions used for importing and exporting the book list
 * to files on the filesystem.
 *
 * The first bytes of the file are a size_t corresponding to the number of
 * elements stored in the file.
 *
 * The books are stored in the order that they are given in the list, with the
 * fields being in the following order:
 *
 * book_id (long)
 * author_id (long)
 * year (int)
 * num_pages (int)
 * quality (float)
 * number of characters in title string (size_t)
 * number of characters in name string (size_t)
 * number of characters in surname string (size_t)
 * title (char *)
 * name (char *)
 * surname (char *)
 *
 * The next functions deal with reading data of various types from stdin.
 */

#define _early_return_ne(x,y) if(x != y){return -1;}
#define _early_return_null(x) if(x == NULL){return -1;}

#define ERROR_INVALID_NUMBER "\tInvalid. Expected number, got '%s'. Please input number: "

struct book *load_books_from_fp(FILE *);
int save_books_to_fp(struct book *, FILE *);

int export_books(struct book *head, char *filename)
{
	FILE *fp = fopen(filename, "w");
	_early_return_null(fp);

	int status = save_books_to_fp(head, fp);
	fclose(fp);

	return status;
}

struct book *import_books(char *filename)
{
	struct book *head;
	FILE *fp = fopen(filename, "r");
	assert(fp != NULL);

	head = load_books_from_fp(fp);
	fclose(fp);

	return head;
}

size_t book_list_length(struct book *head)
{
	size_t length = 0;
	while (head != NULL)
	{
		length += 1;
		head = head->next;
	}

	return length;
}

int save_books_to_fp(struct book *head, FILE *fp)
{
	size_t elements_written, title_chars, name_chars, surname_chars, length;

	length = book_list_length(head);
	elements_written = fwrite(&length, sizeof(size_t), 1, fp);
	_early_return_ne(elements_written, 1);

	while (head != NULL) {
		elements_written = fwrite(&(head->l_book_id), sizeof(long), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(head->l_author_id), sizeof(long), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(head->i_year), sizeof(int), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(head->i_numb_pages), sizeof(int), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(&(head->f_quality), sizeof(float), 1, fp);
		_early_return_ne(elements_written, 1);

		title_chars = strlen(head->ptr_title);
		elements_written = fwrite(&title_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		name_chars = strlen(head->ptr_name);
		elements_written = fwrite(&name_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		surname_chars = strlen(head->ptr_surname);
		elements_written = fwrite(&surname_chars, sizeof(size_t), 1, fp);
		_early_return_ne(elements_written, 1);

		elements_written = fwrite(head->ptr_title, sizeof(char), title_chars, fp);
		_early_return_ne(elements_written, title_chars);

		elements_written = fwrite(head->ptr_name, sizeof(char), name_chars, fp);
		_early_return_ne(elements_written, name_chars);

		elements_written = fwrite(head->ptr_surname, sizeof(char), surname_chars, fp);
		_early_return_ne(elements_written, surname_chars);

		head = head->next;
	}

	return 0;
}

struct book *load_book_from_fp(FILE *fp)
{
	size_t bytes_read, title_chars, name_chars, surname_chars;
	struct book *b;
	if (feof(fp))
	{
		return NULL;
	}

	b = malloc(sizeof(struct book));
	b->next = NULL;

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

	return b;
}

struct book *load_books_from_fp(FILE *fp)
{
	struct book *head, *current;
	size_t elements, i;

	if (feof(fp))
	{
		return NULL;
	}

	i = fread(&elements, sizeof(size_t), 1, fp);
	/*assert(i */

	head = load_book_from_fp(fp);
	current = head;

	for (i = 1; i < elements; i++)
	{
		current->next = load_book_from_fp(fp);
		current = current->next;
	}

	return head;
}

long get_long()
{
	char *aux = get_string();
	char *aux2;
	long out = strtol(aux, &aux2, 10);

	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_int();
	}
	else
	{
		free(aux);
	}

	return out;
}

int get_int()
{
	char *aux = get_string();
	char *aux2;
	int out = strtol(aux, &aux2, 10);


	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_int();
	}
	else
	{
		free(aux);
	}

	return out;
}

float get_float()
{
	char *aux = get_string();
	char *aux2;
	float out = strtof(aux, &aux2);

	if (aux2 == aux) {
		printf(ERROR_INVALID_NUMBER, aux);
		free(aux);
		return get_int();
	}
	else
	{
		free(aux);
	}

	return out;
}

char *get_string()
{
	size_t size = 0;
	char *ptr = NULL;
	size = getline(&ptr, &size, stdin);
	ptr[size-1] = '\0';

	return ptr;
}
