//MainFunction my_book_manager project
//Developer:
//Dev_Tester:
//Integrator:
//Int_Tester:
//Date:4/11/15
//==================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "book.h"
#include "io.h"
#include "autosave.h"

//===================================================================================================================================================================

#ifndef DEFAULT_FILENAME
#define DEFAULT_FILENAME "data_demo.sbm"
#endif /* DEFAULT_FILENAME */

struct prog_info
{
	struct book_node *first;
	int cmd_count;
	pthread_t *autosave_thread;
	pthread_mutex_t book_mutex;
	struct save_thread_args autosave_args;
};

//Prototypes
void show_menu(struct prog_info *info);
void insert(struct prog_info *info);
void search_and_update(struct book_node *);
void change(struct book *ptr, struct book_node *);
void change_id(struct book *ptr, struct book_node *head);
void change_title(struct book *ptr);
void change_year(struct book *ptr);
void change_pages(struct book *ptr);
void change_quality(struct book *ptr);
void change_author_id(struct book *ptr, struct book_node *);
void change_author_name(struct book *ptr, struct book_node *);
void change_author_surname(struct book *ptr, struct book_node *);
void recursive_modification(struct book *ptr, struct book_node *);
void prompt_remove_book(struct prog_info *info);
static void cleanup_new_book(struct book *x);

int main(void)
{
	struct prog_info info;

	info.first = NULL;
	info.cmd_count = 0;

	int status = pthread_mutex_init(&info.book_mutex, NULL);
	if (status != 0)
	{
		printf("FAILED TO INITIALIZE MUTEX, INIT RETURNED %d\n", status);
		exit(1);
	}

	info.autosave_args.filename = NULL;
	info.autosave_args.book_mutex = &info.book_mutex;
	info.autosave_args.head = NULL;

	info.autosave_thread = NULL;

	show_menu(&info);

	return 0;
}


void show_menu(struct prog_info *info)
{
	int opc, get_status;
	struct book_node *new_list;
#ifdef DEBUG
	/*
	 * If debugging is enabled, then we need this variable to hold onto the
	 * filename that the user enters, otherwise, we're using the default
	 * filename of DEFAULT_FILENAME, which can be overwritten at compile-time
	 * with -DDEFAULT_FILENAME=other_name.dat
	 */
	char *filename;
#else
	char filename[] = DEFAULT_FILENAME;
#endif /* DEBUG */

	do {
		printf("==================================================");
		printf("\n[*][2015][SAUCEM APPS][My Books] =================");
		printf("\n==================================================");
		printf("\n[0]\tExit\n[1]\tInsert a book\n[2]\tShow all books\n[3]\tShow a book by ID\n");
		printf("[4]\tmodify a book by ID\n[5]\tremove a book by ID\n[6]\tExport catalogue\n");
		printf("[7]\tImport Catalogue\n[8]\tShow corrupt records\n[9]\t");

		if (info->autosave_thread == NULL)
		{
			printf("Activate autosave (every %d seconds)", AUTOSAVE_SECONDS);
		}
		else
		{
			printf("Deactivate autosave");
		}

#ifdef DEBUG
		printf("\n\n[DEBUG] %d commands run.", info->cmd_count);
#endif /* DEBUG */
		printf("\n   \tType your option [0-9]: ");

		do {
			get_status = get_int(&opc);
			if (get_status == GET_USR_ENTERED_CTRLD)
			{
				pthread_mutex_lock(&info->book_mutex);
				free_list(info->first);
				stop_autosave(info->autosave_thread, &info->autosave_args);
				exit(0);
			}

			if (opc <0 || opc >9)
			{
				printf("Please only introduce values from 0 to 9");
			}
		}while (opc <0 || opc >9);

		switch(opc) {
			case 1:
				pthread_mutex_lock(&info->book_mutex);
				insert(info);
				pthread_mutex_unlock(&info->book_mutex);
				break;
			case 2:
				print_books(info->first);
				break;
			case 3:
				printf("\nPlease Introduce the Id of the book:");
				long id;
				get_status = get_long(&id);

				if (get_status == GET_USR_ENTERED_CTRLD)
				{
					pthread_mutex_lock(&info->book_mutex);
					free_list(info->first);
					stop_autosave(info->autosave_thread, &info->autosave_args);
					pthread_mutex_destroy(&info->book_mutex);
					exit(0);
				}

				print_book(find_by_id(info->first, id));
				break;
			case 4:
				pthread_mutex_lock(&info->book_mutex);
				search_and_update(info->first);
				pthread_mutex_unlock(&info->book_mutex);
				break;
			case 5:
				pthread_mutex_lock(&info->book_mutex);
				prompt_remove_book(info);
				pthread_mutex_unlock(&info->book_mutex);
				break;
			case 6:
#ifdef DEBUG
				printf("[DEBUG] Please Introduce filename: ");

				filename = get_string();

				if (filename == NULL)
				{
					pthread_mutex_lock(&info->book_mutex);
					free_list(info->first);
					stop_autosave(info->autosave_thread, &info->autosave_args);
					pthread_mutex_destroy(&info->book_mutex);
					exit(0);
				}

				export_books(info->first, filename);
				free(filename);
#else /* DEBUG */
				export_books(info->first, filename);
#endif /* DEBUG */

				break;
			case 7:
#ifdef DEBUG
				printf("[DEBUG] Please Introduce filename: ");

				filename = get_string();
				if (filename == NULL)
				{
					pthread_mutex_lock(&info->book_mutex);
					free_list(info->first);
					stop_autosave(info->autosave_thread, &info->autosave_args);
					pthread_mutex_destroy(&info->book_mutex);
					exit(0);
				}
				new_list = import_books(filename);
#else /* DEBUG */
				new_list = import_books(filename);
#endif /* DEBUG */
				if (new_list == NULL)
				{
					printf("Could not load list from file '%s'. Keeping old list.\n", filename);
				}
				else
				{

					pthread_mutex_lock(&info->book_mutex);
					free_list(info->first);
					info->first = new_list;
					pthread_mutex_unlock(&info->book_mutex);
				}
#ifdef DEBUG
				free(filename);
#endif /* DEBUG */
				break;
			case 8:
				display_corrupt_records(info->first);
				break;
			case 9:
				if (info->autosave_thread == NULL)
				{
					info->autosave_thread = malloc(sizeof(pthread_t));
					info->autosave_args.head = info->first;
					start_autosave(info->autosave_thread, &info->autosave_args);
				}
				else
				{
					stop_autosave(info->autosave_thread, &info->autosave_args);
					info->autosave_thread = NULL;
				}
				break;
			case 0:
				stop_autosave(info->autosave_thread, &info->autosave_args);
				free_list(info->first);
				pthread_mutex_destroy(&info->book_mutex);
				return;
		}

		info->cmd_count++;
	} while (opc != 0);

}

void insert(struct prog_info *info)
{
	struct book *new = new_book();
	int status;

	//Now we fill the fields of the book.
	printf("\nIntroduce the following information:\n");
	printf("\t\tthe Id of the book: ");
	status = get_long(&new->l_book_id);
	if (status == GET_USR_ENTERED_CTRLD)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe title of the book: ");
	new->ptr_title = get_string();
	if (new->ptr_title == NULL)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe publication year: ");
	status = get_int(&new->i_year);
	if (status == GET_USR_ENTERED_CTRLD)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe quality of the book: ");
	status = get_float(&new->f_quality);
	if (status == GET_USR_ENTERED_CTRLD)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe Id of the author: ");
	status = get_long(&new->l_author_id);
	if (status == GET_USR_ENTERED_CTRLD)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe name of the author: ");
	new->ptr_name = get_string();
	if (new->ptr_name == NULL)
	{
		cleanup_new_book(new);
		return;
	}

	printf("\t\tthe surname of the author: ");
	new->ptr_surname = get_string();
	if (new->ptr_surname == NULL)
	{
		cleanup_new_book(new);
		return;
	}

	info->first = append(info->first, new);
	return;
}

static void cleanup_new_book(struct book *x)
{
	printf("Got CTRL-D, returning to menu...\n");
	free_book(x);
}

void search_and_update(struct book_node *head)
{
	printf("\nPlease introduce the ID of the book you wish to modify: ");
	long id;
	int status = get_long(&id);

	if (status == GET_USR_ENTERED_CTRLD)
	{
		return;
	}

	struct book *ptr = find_by_id(head, id);
	if (ptr == NULL)
	{
		printf("There is no book with id: %ld", id);
		search_and_update(head);
	}
	else
	{
		change(ptr, head);
	}
}

void change(struct book *ptr, struct book_node *head)
{
	printf("\nWhich field do you want to modify?\n");
	printf("\t[1]Book ID\n");
	printf("\t[2]Book Title\n");
	printf("\t[3]Year of publication\n");
	printf("\t[4]Number of Pages\n");
	printf("\t[5]Quality of the book\n");
	printf("\t[6]Author ID\n");
	printf("\t[7]Author Name\n");
	printf("\t[8]Author Surname\n");
	int opc, status;
	do {

		status = get_int(&opc);
		if (status == GET_USR_ENTERED_CTRLD)
		{
			return;
		}
		if (opc<1||opc>8)
		{
			printf("Please only introduce values between 1 and 8:");
		}
	}while (opc<1||opc>8);
	switch(opc) {
		case 1:
			change_id(ptr, head);
			break;
		case 2:
			change_title(ptr);
			break;
		case 3:
			change_year(ptr);
			break;
		case 4:
			change_pages(ptr);
			break;
		case 5:
			change_quality(ptr);
			break;
		case 6:
			change_author_id(ptr, head);
			break;
		case 7:
			change_author_name(ptr, head);
			break;
		case 8:
			change_author_surname(ptr, head);
			break;
	}
}

void change_id(struct book *ptr, struct book_node *head)
{
	printf("\nPlease introduce the new ID of the book:\n");
	long new_id, status;

	status = get_long(&new_id);

	if (status == GET_USR_ENTERED_CTRLD)
	{
		return;
	}

	struct book *check = find_by_id(head, new_id);

	if (check != NULL && check != ptr)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again:\n", check->ptr_title);
		change_id(ptr, head);
	}
	ptr->l_book_id = new_id;
}

void change_title(struct book *ptr)
{
	char *tmp;
	printf("\nPlease introduce the new title of the book:");
	tmp = get_string();

	if (tmp == NULL)
	{
		return;
	}

	free(ptr->ptr_title);
	ptr->ptr_title = tmp;
}

void change_year(struct book *ptr)
{
	int status, year;
	printf("\nPlease introduce the year of publication: ");
	do {
		status = get_int(&year);
		if (status == GET_USR_ENTERED_CTRLD)
		{
			return;
		}
		if (year <= 0)
		{
			printf("\nPlease introduce a valid year: ");
		}
	} while (year <= -1);
	ptr->i_year = year;
}

void change_pages(struct book *ptr)
{
	int status, pages;
	printf("\nPlease introduce the number of pages: ");
	do {
		status = get_int(&pages);
		if (status == GET_USR_ENTERED_CTRLD)
		{
			return;
		}
		if (pages <= 0)
		{
			printf("\nPlease introduce a valid number of pages: ");
		}
	} while (pages <= 0);
	ptr->i_numb_pages = pages;
}

void change_quality(struct book *ptr)
{
	int status;
	float quality;
	printf("\nPlease introduce the Quality: ");
	do {
		status = get_float(&quality);
		if (status == GET_USR_ENTERED_CTRLD)
		{
			return;
		}
		if (quality <= 0)
		{
			printf("\nPlease introduce a valid quality: ");
		}
	} while (quality <= 0);
	ptr->f_quality = quality;
}

void change_author_id(struct book *ptr, struct book_node *head)
{
	printf("\nPlease introduce the new ID of the Author: \n");
	long new_id;
	int status = get_long(&new_id);

	if (status == GET_USR_ENTERED_CTRLD)
	{
		return;
	}

	struct book *check = find_by_author_id(head, new_id);
	if (check != NULL)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again: \n", check->ptr_name);
		change_author_id(ptr, head);
	}
	//We have to change the ID of the author in all the books of the library wrote by that author
	long original_id = ptr->l_author_id;
	while (head != NULL)
	{
		if (head->book->l_author_id == original_id)
		{
			head->book->l_author_id = new_id;
		}
		head = head->next;
	}
}

void change_author_name(struct book *ptr, struct book_node *head)
{
	printf("\nPlease introduce the new name of the author: ");
	char *new_name = get_string();

	if (new_name == NULL)
	{
		return;
	}

	long id = ptr->l_author_id;
	while (head != NULL)
	{
		if (head->book->l_author_id == id)
		{
			free(head->book->ptr_name);
			head->book->ptr_name = malloc(sizeof(char) * strlen(new_name) + 1);
			strcpy(head->book->ptr_name, new_name);
		}
		head = head->next;
	}

	free(new_name);
}

void change_author_surname(struct book *ptr, struct book_node *head)
{
	printf("\nPlease introduce the new surname of the author: ");
	char *new_name = get_string();

	if (new_name == NULL)
	{
		return;
	}

	long id = ptr->l_author_id;
	while (head != NULL)
	{
		if (head->book->l_author_id == id)
		{
			free(head->book->ptr_surname);
			head->book->ptr_surname = malloc(sizeof(char) * strlen(new_name) + 1);
			strcpy(head->book->ptr_surname, new_name);
		}
		head = head->next;
	}

	free(new_name);
}

void recursive_modification(struct book *ptr, struct book_node *head)
{
	printf("\nPress 1 to modify another field of the book.\nPress 2 to choose another book to modify.\nPress 3 to return to main menu: ");
	int opc, status;
	do {

		status = get_int(&opc);
		if (status == GET_USR_ENTERED_CTRLD)
		{
			return;
		}
		if (opc < 1 || opc > 3)
		{
			printf("Please only introduce values 1, 2, or 3:");
		}
	} while (opc < 1 || opc > 3);

	switch(opc) {
		case 1:
			change(ptr, head);
			recursive_modification(ptr, head);
			break;
		case 2:
			search_and_update(head);
			break;
		case 3:
			break;
	}
}

void prompt_remove_book(struct prog_info *info)
{
	printf("\nPlease introduce the ID of the book you wish to remove: ");
	long id;
	int status = get_long(&id);

	if (status == GET_USR_ENTERED_CTRLD)
	{
		return;
	}

	info->first = remove_first_with_id(info->first, id);
}
