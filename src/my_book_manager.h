#ifndef _MY_BOOK_MANAGER_H_
#define _MY_BOOK_MANAGER_H_

#include <pthread.h>

#include "book.h"
#include "autosave.h"

/*---------------------------------------------------------------------------*/
/* Structure declarations                                                    */
/*---------------------------------------------------------------------------*/
/**Struct**********************************************************************

  Synopsis    program information.

******************************************************************************/
struct prog_info
{
	struct book_node *first;
	int cmd_count;
	pthread_t *autosave_thread;
	pthread_mutex_t book_mutex;
	struct save_thread_args autosave_args;
};

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
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
void prompt_remove_book(struct prog_info *info);

#endif /* ifndef _MY_BOOK_MANAGER_H_ */
