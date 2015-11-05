//MainFunction my_book_manager project
//Developer:
//Dev_Tester:
//Integrator:
//Int_Tester:
//Date:4/11/15
//==================================================



//Struct



//Prototypes:



#include<stdio.h>
#include<stdlib.h>

#include "my_book_manager.h"

//global variables:
int cmdCount = 0;
int opc;

int main(void){
  do{
    showMenu();

    if(opc <0 || opc >9){
      printf("ERROR: Please select a valid option\n");
    }else{
      switch(opc) {
	case 1:
	  insert();
	  break;
	case 2:
	  break;
	case 3:
	  break;
	case 4:
	  break;
	case 5:
	  break;
	case 6:
	  break;
	case 7:
	  break;
	case 8:
	  break;
	case 9:
	  break;

      }
    }
  }while((opc != 0));
 return 0;
}


void showMenu(void){


    printf("==================================================");
    printf("\n[*][2015][SAUCEM APPS][My Books] =================");
    printf("\n==================================================");
    printf("\n[0]\tExit\n[1]\tInsert a book\n[2]\tShow all books\n[3]\tShow a book by ID\n");
    printf("[4]\tModify a book by ID\n[5]\tRemove a book by ID\n[6]\tExport catalogue\n");
    printf("[7]\tImport Catalogue\n[8]\tShow corrupt records\n[9]\tActivate autosave (every 10 seconds)");
    printf("\n\n   \t#INFO: %d commands executed.\n   \tType your option [0-9]:", cmdCount);
    scanf("%d",&opc);

    cmdCount++;
    return;
 }
 void insert(void){
   size_t sentenceLenght=0;//initialized to 0
   char *ptr;//pointer which stores the fist chars direction of the sentence
   struct book bookNode = *new_book();
   struct book *ptrNode;
   ptrNode = &bookNode;

   /*
   PROBLEMS:
   1)the getline isnt working properly as in the first loop it goes over it and directly prints
   the error: introduce a number message.I figured out that it is because as it doesnt work,
   the ptr doesnt have a number, so the atoi returns 0, and finally it prints the message.
   2)we cant assing float from char in the quality.
   3)I am not sure if im correctly assigning the values of the pointer after the getline(supposing it worked)
   to the parameters of the node
   */
   //==================================================

   printf("\nIntroduce the following information:\n\t\t");
   printf("the Id of the book: ");
   long ID;

   do{
      do{
	  sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
	  if(sentenceLenght==-1){
	      printf("\n\t\tERROR allocating memory\n");
	  }
      }while(sentenceLenght==-1);
      ID=atoi(ptr);
      if(ID==0){printf("\n\t\tERROR. Introduce a number\n");}
   }while(ID==0);
   ptrNode->l_book_id = (long)ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe title of the book: ");
   do{
      sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
      if(sentenceLenght==-1){
	  printf("\nERROR allocating memory\n");
      }
   }while(sentenceLenght==-1);
   ptrNode->ptr_title = ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe publication year: ");
   int year;
   do{
      do{
	  sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
	  if(sentenceLenght==-1){
	      printf("\n\t\tERROR allocating memory\n");
	  }
      }while(sentenceLenght==-1);
      year=atoi(ptr);
      if(year==0||year<=0){printf("\n\t\tERROR. Introduce a number\n");}
   }while(year==0||year<=0);
   ptrNode->i_year = (long)ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe quality of the book: ");
   float f_quality;
   do{
      do{
      sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
	  if(sentenceLenght==-1){
	      printf("\n\t\tERROR allocating memory\n");
	  }
      }while(sentenceLenght==-1);
      f_quality=atoi(ptr);
      if(f_quality==0|| f_quality<=0){printf("\n\t\tERROR. Introduce a number\n");}
   }while(f_quality==0|| f_quality<=0);
   //ptrNode->f_quality = ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe Id of the author: ");
   long ID_Author;
   do{
      do{
	  sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
	  if(sentenceLenght==-1){
	      printf("\n\t\tERROR allocating memory\n");
	  }
      }while(sentenceLenght==-1);
      ID_Author=atoi(ptr);
      if(ID==0){printf("\n\t\tERROR. Introduce a number\n");}
   }while(ID==0);
   ptrNode->l_author_id = (long)ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe name of the author: ");
   do{
      sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
      if(sentenceLenght==-1){
	  printf("\n\t\tERROR allocating memory\n");
      }
   }while(sentenceLenght==-1);
   ptrNode->ptr_name = ptr;
   sentenceLenght=0;

   //==================================================

   printf("\t\tthe surname of the author: ");
   do{
      sentenceLenght=getline(&ptr/*double pointer*/,&sentenceLenght,stdin);
      if(sentenceLenght==-1){
	  printf("\n\t\tERROR allocating memory\n");
      }
   }while(sentenceLenght==-1);
   ptrNode->ptr_surname = ptr;

   //==================================================

   free(ptr);//we free the dynamicly allocated memory stored by getline in order to prevent memory leaks
   return;
}

struct book *new_book(void)
{
	struct book *book = calloc(sizeof(struct book), 1);


	return book;
}
