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
#include "multiproc.h"

//===================================================================================================================================================================

struct prog_info
{
	struct book *first;
	struct book *last;
	int cmdCount;
	int DynamicCount;
	pthread_t *autosave_thread;
};

//Prototypes
void show_menu(struct prog_info *info);
void insert(struct prog_info *info);
void show_summary(struct prog_info *info);
void show_publication (long ID, struct prog_info *info);
void modify (struct prog_info *info);
void change(struct book *ptr, struct prog_info *info);
void change_ID(struct book *ptr, struct prog_info *info);
void change_title(struct book *ptr, struct prog_info *info);
void change_year(struct book *ptr, struct prog_info *info);
void change_pages(struct book *ptr, struct prog_info *info);
void change_quality(struct book *ptr, struct prog_info *info);
void change_author_ID(struct book *ptr, struct prog_info *info);
void change_name(struct book *ptr, struct prog_info *info);
void change_surname(struct book *ptr, struct prog_info *info);
void recursive_modification(struct book *ptr, struct prog_info *info);
void remove_book(struct prog_info *info);
struct book *next_ID_search(long ID, struct book *first);

int main(void)
{
	struct prog_info info;

	info.first = NULL;
	info.cmdCount = 0;
	info.DynamicCount = 0;
	info.autosave_thread = NULL;

	show_menu(&info);

	return 0;
}


void show_menu(struct prog_info *info)
{
	int opc;
	struct book *new_list;
	char *filename;

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

		printf("\n\n   \t#INFO: %d commands executed.\n   \tType your option [0-9]:", info->cmdCount);

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
				show_summary(info);
				break;
			case 3:
				printf("\nPlease Introduce the Id of the book:");
				long ID = get_long();
				show_publication(ID, info);
				break;
			case 4:
				modify(info);
				break;
			case 5:
				remove_book(info);
				break;
			case 6:
				printf("\tPlease Introduce filename: ");

				filename = get_string();
				export_books(info->first, filename);

				free(filename);
				break;
			case 7:
				printf("\tPlease Introduce filename: ");

				filename = get_string();
				new_list = import_books(filename);

				free_list(info->first);
				free(filename);

				info->first = new_list;
				break;
			case 8:
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

		info->cmdCount++;
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

	append(&info->first, ptr_new);
	return;
}

void show_summary(struct prog_info *info)
{
	struct book *tmp = info->first;

	printf("\n===============================================================");
	printf("======================================\n== ""Book id"" |""  Títle (20 chars )""| ""Author id"" ==========================================================");
	printf("\n===============================================================");
	printf("======================================");
	printf("\n===============================================================");
	printf("======================================\n");

	while (tmp != NULL)
	{
		printf("== %-ld |%-20s|%-ld\n", tmp->l_book_id, tmp->ptr_title, tmp->l_author_id);
		tmp = tmp->next;
	}
	printf("\n=====================================================");
	printf("================================================\n");
}

void show_publication(long id, struct prog_info *info)
{
	struct book *ptr = find_by_id(info->first, id);
	if (ptr == NULL) {
		printf("No book with id %ld exists.\n", id);
		return;
	}

	printf("Detailed information for==(%ld)==\n", id);
	printf("=%ld= %31s %ld\n", ptr->l_book_id, "Identifier:", ptr->l_book_id);
	printf("=%ld= %31s %s\n", ptr->l_book_id, "Title of the book:", ptr->ptr_title);
	printf("=%ld= %31s %i\n", ptr->l_book_id, "Publication year:", ptr->i_year);
	printf("=%ld= %31s %i\n", ptr->l_book_id, "Number of pages:", ptr->i_numb_pages);
	printf("=%ld= %31s %lf\n", ptr->l_book_id, "Quality:", ptr->f_quality);
	printf("=%ld= %31s %ld\n", ptr->l_book_id, "Authors' ID:", ptr->l_author_id);
	printf("=%ld= %31s %s\n", ptr->l_book_id, "Name of the author:", ptr->ptr_name);
	printf("=%ld= %31s %s\n", ptr->l_book_id, "Surname of the authour:", ptr->ptr_surname);
}

void modify (struct prog_info *info)
{
	printf("\nPlease introduce the ID of the book you wish to modify:\n");
	long id = get_long();
	struct book *ptr = find_by_id(info->first, id);
	change(ptr, info);
}

void change(struct book *ptr, struct prog_info *info)
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
			change_ID(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 2:
			change_title(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 3:
			change_year(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 4:
			change_pages(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 5:
			change_quality(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 6:
			change_author_ID(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 7:
			change_name(ptr, info);
			recursive_modification(ptr, info);
			break;
		case 8:
			change_surname(ptr, info);
			recursive_modification(ptr, info);
			break;
	}
}

void change_ID(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the new ID of the book:\n");
	long new_id = get_long();
	struct book *check = find_by_id(info->first, new_id);
	if (check!=NULL)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again:\n", check->ptr_title);
		change_ID(ptr, info);
	}
	ptr->l_book_id = new_id;
}

void change_title(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the new title of the book:");
	ptr->ptr_title=get_string();

}

void change_year(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the year of publication:");
	do {
		ptr->i_year=get_int();
		if (ptr->i_year<=0)
		{
			printf("\n Please introduce a valid year:");
		}
	}while (ptr->i_year<=0);
}

void change_pages(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the number of pages:");
	do {
		ptr->i_year=get_int();
		if (ptr->i_year<=0)
		{
			printf("\n Please introduce a valid number of pages:");
		}
	}while (ptr->i_year<=0);
}

void change_quality(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the Quality:");
	do {
		ptr->i_year=get_float();
		if (ptr->i_year<=0)
		{
			printf("\n Please introduce a valid quality:");
		}
	}while (ptr->i_year<=0);
}

void change_author_ID(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the new ID of the Author:\n");
	long new_id = get_long();
	struct book *check = find_by_author_id(info->first, new_id);
	if (check != NULL)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again:\n", check->ptr_name);
		change_author_ID(ptr, info);
	}
	//We have to change the ID of the author in all the books of the library wrote by that author
	long originalID=ptr->l_author_id;
	while (info->first->next!=NULL)
	{
		if (info->first->l_author_id==originalID)
		{
			info->first->l_author_id = new_id;
		}
		info->first=info->first->next;
	}
}


void change_name(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the new name of the author:");
	char *newName=get_string();
	char * originalName=ptr->ptr_name;
	while (info->first->next!=NULL)
	{
		if (info->first->ptr_name==originalName)
		{
			info->first->ptr_surname=newName;
		}
		info->first=info->first->next;
	}

}

void change_surname(struct book *ptr, struct prog_info *info)
{
	printf("\nPlease introduce the new surname of the author:");
	char *newSurname=get_string();
	char * originalSurname=ptr->ptr_surname;
	while (info->first->next!=NULL)
	{
		if (info->first->ptr_surname==originalSurname)
		{
			info->first->ptr_surname=newSurname;
		}
		info->first=info->first->next;

	}
}

void recursive_modification(struct book *ptr, struct prog_info *info)
{
	printf("\nPress 1 to modify another field of the book.\nPress 2 to choose another book to modify.\nPress 3 to return to main menu.");
	int opc;
	do {

		opc=get_int();
		if (opc!=1||opc!=2||opc!=3)
		{
			printf("Please only introduce values 1, 2, or 3:");
		}
	}while (opc!=1&&opc!=2&&opc!=3);
	switch(opc) {
		case 1:
			change(ptr, info);
			break;
		case 2:
			modify(info);
			break;
		case 3:
			show_menu(info);
			break;
	}
}

void remove_book(struct prog_info *info)
{
	printf("\nPlease introduce the ID of the book you wish to remove:\n");
	long id = get_long();

	info->first = remove_by_id(info->first, id);
}
