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

int main(void)
{
	struct prog_info info;

	info.first = NULL;
	info.cmd_count = 0;
	info.autosave_thread = NULL;

	show_menu(&info);

	return 0;
}


void show_menu(struct prog_info *info)
{
	int opc;
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
	char filename[] = DEFAULT_AUTOSAVE_FILENAME;
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
			opc=get_int();
			if (opc <0 || opc >9)
			{
				printf("Please only introduce values from 0 to 9");
			}
		}while (opc <0 || opc >9);

		switch(opc) {
			case 1:
				insert(info);
				break;
			case 2:
				print_books(info->first);
				break;
			case 3:
				printf("\nPlease Introduce the Id of the book:");
				long id = get_long();
				print_book(find_by_id(info->first, id));
				break;
			case 4:
				search_and_update(info->first);
				break;
			case 5:
				prompt_remove_book(info);
				break;
			case 6:
#ifdef DEBUG
				printf("[DEBUG] Please Introduce filename: ");

				filename = get_string();
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
					free_list(info->first);
					info->first = new_list;
				}
#ifdef DEBUG
				free(filename);
#endif /* DEBUG */

				info->first = new_list;
				break;
			case 8:
				display_corrupt_records(info->first);
				break;
			case 9:
				if (info->autosave_thread == NULL)
				{
					info->autosave_thread = malloc(sizeof(pthread_t));
					start_autosave(info->autosave_thread, info->first);
				}
				else
				{
					stop_autosave(info->autosave_thread);
					info->autosave_thread = NULL;
				}
				break;
			case 0:
				if (info->autosave_thread != NULL)
				{
					stop_autosave(info->autosave_thread);
				}
				free_list(info->first);
				return;
		}

		info->cmd_count++;
	}while (opc!=0);

}

void insert(struct prog_info *info)
{
	struct book *ptr_new = new_book();

	//Now we fill the fields of the book.
	printf("\nIntroduce the following information:\n");
	printf("\t\tthe Id of the book: ");
	ptr_new->l_book_id = get_long();

	printf("\t\tthe title of the book: ");
	ptr_new->ptr_title = get_string();

	printf("\t\tthe publication year: ");
	ptr_new->i_year = get_int();

	printf("\t\tthe quality of the book: ");
	ptr_new->f_quality = get_float();

	printf("\t\tthe Id of the author: ");
	ptr_new->l_author_id = get_long();

	printf("\t\tthe name of the author: ");
	ptr_new->ptr_name = get_string();

	printf("\t\tthe surname of the author: ");
	ptr_new->ptr_surname = get_string();

	info->first = append(info->first, ptr_new);
	return;
}

void search_and_update(struct book_node *head)
{
	printf("\nPlease introduce the ID of the book you wish to modify: ");
	long id = get_long();
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
	int opc;
	do {

		opc=get_int();
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
	long new_id = get_long();
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
	printf("\nPlease introduce the new title of the book:");
	free(ptr->ptr_title);
	ptr->ptr_title = get_string();
}

void change_year(struct book *ptr)
{
	printf("\nPlease introduce the year of publication: ");
	do {
		ptr->i_year = get_int();
		if (ptr->i_year <= 0)
		{
			printf("\nPlease introduce a valid year: ");
		}
	} while (ptr->i_year <= 0);
}

void change_pages(struct book *ptr)
{
	printf("\nPlease introduce the number of pages: ");
	do {
		ptr->i_numb_pages = get_int();
		if (ptr->i_year <= 0)
		{
			printf("\nPlease introduce a valid number of pages: ");
		}
	}while (ptr->i_numb_pages <= 0);
}

void change_quality(struct book *ptr)
{
	printf("\nPlease introduce the Quality: ");
	do {
		ptr->f_quality = get_float();
		if (ptr->i_year <= 0)
		{
			printf("\nPlease introduce a valid quality: ");
		}
	}while (ptr->f_quality <= 0);
}

void change_author_id(struct book *ptr, struct book_node *head)
{
	printf("\nPlease introduce the new ID of the Author: \n");
	long new_id = get_long();
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
	int opc;
	do {

		opc = get_int();
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
	long id = get_long();

	info->first = remove_first_with_id(info->first, id);
}
