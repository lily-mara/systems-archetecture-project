//MainFunction my_book_manager project
//Developer:
//Dev_Tester:
//Integrator:
//Int_Tester:
//Date:4/11/15
//==================================================


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "io.h"

#include "my_book_manager.h"

//global variables:
int cmdCount = 0;
int DynamicCount=0;//global variable that takes the account of memory leaks

//===================================================================================================================================================================

//Prototypes
void ShowMenu(struct book *first);
void insert(struct book *first);
struct book *new_book(void);
struct book *BookSearch(long ID,struct book *current/*pass first book as parameter*/);
void ShowSummary(struct book *first);
void ShowPublication (long ID,struct book *first);
struct book *AuthorSearch(long ID,struct book *current/*pass first book as parameter*/);
void Modify (struct book *first);
void Change(struct book *ptr,struct book *first);
long getLong();
int getInt();
float getFloat();
char * getString();
void ChangeID(struct book *ptr,struct book *first);
void ChangeTitle(struct book *ptr);
void ChangeYear(struct book *ptr);
void ChangePages(struct book *ptr);
void ChangeQuality(struct book *ptr);
void ChangeAuthorID(struct book *ptr,struct book *first);
void ChangeName(struct book *ptr,struct book *first);
void ChangeSurname(struct book *ptr,struct book *first);
void RecursiveModification(struct book *ptr,struct book *first);
void Remove(struct book *first);
struct book *NextIDSearch(long ID,struct book *first);


void ShowMenu(struct book *first)
{
	int opc;
	do {
		printf("==================================================");
		printf("\n[*][2015][SAUCEM APPS][My Books] =================");
		printf("\n==================================================");
		printf("\n[0]\tExit\n[1]\tInsert a book\n[2]\tShow all books\n[3]\tShow a book by ID\n");
		printf("[4]\tModify a book by ID\n[5]\tRemove a book by ID\n[6]\tExport catalogue\n");
		printf("[7]\tImport Catalogue\n[8]\tShow corrupt records\n[9]\tActivate autosave (every 10 seconds)");
		printf("\n\n   \t#INFO: %d commands executed.\n   \tType your option [0-9]:", cmdCount);

		do {
			opc=getInt();
			if(opc <0 || opc >9)
			{
				printf("Please only introduce values from 0 to 9");
			}
		} while(opc <0 || opc >9);

		switch(opc) {
			case 1:
				insert(first);
				ShowMenu(first);
				break;
			case 2:
				ShowSummary(first);
				ShowMenu(first);
				break;
			case 3:
				printf("\nPlease Introduce the Id of the book:");
				long ID=getLong();
				ShowPublication(ID,first);
				ShowMenu(first);
				break;
			case 4:
				Modify(first);
				ShowMenu(first);
				break;
			case 5:
				Remove(first);
				ShowMenu(first);
				break;
			case 6:
				save_books_to_filename(first, "books.dat");
				break;
			case 7:
				first = load_books_from_filename("books.dat");
				break;
			case 8:

				break;
			case 9:

				break;

		}

		cmdCount++;
		return;
	} while(opc!=0);

}


void insert(struct book *first)
{
	struct book *ptrNode= new_book();
	ptrNode->next=NULL;
	//Now we introduce this new book at the end of the list:
	do {
		if(first->next==NULL)
		{
			first->next=ptrNode;
		}
		first=first->next;
	} while(first->next!=NULL);

	//Now we fill the fields of the book.
	printf("\nIntroduce the following information:\n\t\t");
	printf("the Id of the book: ");
	ptrNode->l_book_id =getLong();

	//==================================================

	printf("\t\tthe title of the book: ");
	ptrNode->ptr_title =getString();


	//==================================================

	printf("\t\tthe publication year: ");
	ptrNode->i_year =getInt();
	//==================================================

	printf("\t\tthe quality of the book: ");
	ptrNode->f_quality =getFloat();
	//==================================================

	printf("\t\tthe Id of the author: ");
	ptrNode->l_author_id =getLong();

	//==================================================

	printf("\t\tthe name of the author: ");
	ptrNode->ptr_name=getString();

	//==================================================

	printf("\t\tthe surname of the author: ");
	ptrNode->ptr_surname=getString();

	//==================================================


	return;
}


struct book *new_book(void)
{
	struct book *book = calloc(sizeof(struct book), 1);

	DynamicCount++;
	return book;
}


struct book *BookSearch(long ID,struct book *current/*pass first book as parameter*/)
{
	if(current->l_book_id==ID)
	{
		return current;
	}
	else if(current->l_book_id!=ID&&current->next!=NULL)
	{
		return BookSearch(ID,current->next);
	}
	else if (current->next==NULL)
	{
		printf("The ID: %ld doesnt match any book in the librery",ID);
		return NULL;
	}
	return NULL;
}


void ShowSummary(struct book *first)
{
	printf("\n===============================================================");
	printf("======================================\n== ""Book id"" |""  Títle (20 chars )""| ""Author id"" ==========================================================");
	printf("\n===============================================================");
	printf("======================================");
	printf("\n===============================================================");
	printf("======================================\n");

	while(first->next!=NULL)
	{
		printf("== %-ld |%-20s|%-ld\n",first->l_book_id,first->ptr_title,first->l_author_id);
		first=first->next;
	}
	printf("\n=====================================================");
	printf("================================================\n");
}


void ShowPublication (long ID,struct book *first)
{
	struct book *ptr=BookSearch(ID,first);
	printf("Detailed information for==(%ld)==\n",ID);
	printf("=%ld= %31s %ld\n",ptr->l_book_id,"Identifier:",ptr->l_book_id);
	printf("=%ld= %31s %s\n",ptr->l_book_id,"Title of the book:",ptr->ptr_title);
	printf("=%ld= %31s %i\n",ptr->l_book_id,"Publication year:",ptr->i_year);
	printf("=%ld= %31s %i\n",ptr->l_book_id,"Number of pages:",ptr->i_numb_pages);
	printf("=%ld= %31s %lf\n",ptr->l_book_id,"Quality:",ptr->f_quality);
	printf("=%ld= %31s %ld\n",ptr->l_book_id,"Authors' ID:",ptr->l_author_id);
	printf("=%ld= %31s %s\n",ptr->l_book_id,"Name of the author:",ptr->ptr_name);
	printf("=%ld= %31s %s\n",ptr->l_book_id,"Surname of the authour:",ptr->ptr_surname);
}


struct book *AuthorSearch(long ID,struct book *current/*pass first book as parameter*/)
{
	if(current->l_author_id==ID)
	{
		return current;
	}
	else if(current->l_author_id!=ID&&current->next!=NULL)
	{
		return AuthorSearch(ID,current->next);
	}
	else if (current->next==NULL)
	{
		printf("The ID: %ld doesnt match any author",ID);
		return NULL;
	}
	return NULL;
}


void Modify (struct book *first)
{
	printf("\nPlease introduce the ID of the book you wish to modify:\n");
	long ID=getLong();
	struct book *ptr=BookSearch(ID,first);
	Change(ptr,first);
}


void Change(struct book *ptr,struct book *first)
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
		opc=getInt();
		if(opc<1||opc>8)
		{
			printf("Please only introduce values between 1 and 8:");
		}
	} while(opc<1||opc>8);
	switch(opc) {
		case 1:
			ChangeID(ptr,first);
			RecursiveModification(ptr,first);
			break;
		case 2:
			ChangeTitle(ptr);
			RecursiveModification(ptr,first);
			break;
		case 3:
			ChangeYear(ptr);
			RecursiveModification(ptr,first);
			break;
		case 4:
			ChangePages(ptr);
			RecursiveModification(ptr,first);
			break;
		case 5:
			ChangeQuality(ptr);
			RecursiveModification(ptr,first);
			break;
		case 6:
			ChangeAuthorID(ptr,first);
			RecursiveModification(ptr,first);
			break;
		case 7:
			ChangeName(ptr,first);
			RecursiveModification(ptr,first);
			break;
		case 8:
			ChangeSurname(ptr,first);
			RecursiveModification(ptr,first);
			break;
	}
}


long getLong()
{
	//This function stores a long using getline
	char *aux=getString();
	char*aux2;
	return strtol(aux,&aux2,10);

}


int getInt()
{
	//This function stores an int using getline
	char *aux=getString();
	char*aux2;
	return strtol(aux,&aux2,10);
}


float getFloat()
{
	//This function stores a float using getline
	char *aux=getString();
	char*aux2;
	return strtol(aux,&aux2,10);
}


char *getString()
{
	//This function stores a string using getline
	size_t size=0;
	char *ptr=NULL;//pointer which stores the position of the first char of the string
	size=getline(&ptr,&size,stdin);
	DynamicCount++;
	ptr[size-1]='\0';
	return ptr;
}


void ChangeID(struct book *ptr,struct book *first)
{
	printf("\nPlease introduce the new ID of the book:\n");
	long newID=getLong();
	struct book *check=BookSearch(newID,first);
	if(check!=NULL)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again:\n",check->ptr_title);
		ChangeID(ptr,first);
	}
	ptr->l_book_id=newID;
}


void ChangeTitle(struct book *ptr)
{
	printf("\nPlease introduce the new title of the book:");
	ptr->ptr_title=getString();

}


void ChangeYear(struct book *ptr)
{
	printf("\nPlease introduce the year of publication:");
	do {
		ptr->i_year=getInt();
		if(ptr->i_year<=0)
		{
			printf("\n Please introduce a valid year:");
		}
	} while(ptr->i_year<=0);
}


void ChangePages(struct book *ptr)
{
	printf("\nPlease introduce the number of pages:");
	do {
		ptr->i_year=getInt();
		if(ptr->i_year<=0)
		{
			printf("\n Please introduce a valid number of pages:");
		}
	} while(ptr->i_year<=0);
}


void ChangeQuality(struct book *ptr)
{
	printf("\nPlease introduce the Quality:");
	do {
		ptr->i_year=getFloat();
		if(ptr->i_year<=0)
		{
			printf("\n Please introduce a valid quality:");
		}
	} while(ptr->i_year<=0);
}


void ChangeAuthorID(struct book *ptr,struct book *first)
{
	printf("\nPlease introduce the new ID of the Author:\n");
	long newID=getLong();
	struct book *check=BookSearch(newID,first);
	if(check!=NULL)
	{
		printf("The ID you have introduced matches the ID of %s\nPlease try again:\n",check->ptr_name);
		ChangeAuthorID(ptr,first);
	}
	//We have to change the ID of the author in all the books of the library wrote by that author
	long originalID=ptr->l_author_id;
	while(first->next!=NULL)
	{
		if(first->l_author_id==originalID)
		{
			first->l_author_id=newID;
		}
		first=first->next;
	}
}


void ChangeName(struct book *ptr,struct book *first)
{
	printf("\nPlease introduce the new name of the author:");
	char *newName=getString();
	char * originalName=ptr->ptr_name;
	while(first->next!=NULL)
	{
		if(first->ptr_name==originalName)
		{
			first->ptr_surname=newName;
		}
		first=first->next;
	}

}


void ChangeSurname(struct book *ptr,struct book *first)
{
	printf("\nPlease introduce the new surname of the author:");
	char *newSurname=getString();
	char * originalSurname=ptr->ptr_surname;
	while(first->next!=NULL)
	{
		if(first->ptr_surname==originalSurname)
		{
			first->ptr_surname=newSurname;
		}
		first=first->next;

	}

}


void RecursiveModification(struct book *ptr,struct book *first)
{
	printf("\nPress 1 to modify another field of the book.\nPress 2 to choose another book to modify.\nPress 3 to return to main menu.");
	int opc;
	do {
		opc=getInt();
		if(opc!=1||opc!=2||opc!=3)
		{
			printf("Please only introduce values 1, 2, or 3:");
		}
	} while(opc!=1&&opc!=2&&opc!=3);
	switch(opc) {
		case 1:
			Change(ptr,first);
			break;
		case 2:
			Modify(first);
			break;
		case 3:
			ShowMenu(first);
			break;
	}
}


void Remove(struct book *first)
{
	printf("\nPlease introduce the ID of the book you wish to remove:\n");
	long ID=getLong();
	struct book *previous=NextIDSearch(ID,first);
	struct book *ptr=BookSearch(ID,first);
	previous->next=ptr->next;
	free(ptr);
}


struct book *NextIDSearch(long ID,struct book *first)
{
	if(first->next->l_book_id==ID)
	{
		return first;
	}
	else if(first->next->l_book_id!=ID&&first->next->next!=NULL)
	{
		return BookSearch(ID,first->next);
	}
	else if (first->next==NULL)
	{
		return NULL;
	}
	return NULL;
}
