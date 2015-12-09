#ifndef _AUTOSAVE_H_
#define _AUTOSAVE_H_

#include <pthread.h>
#include "book.h"

#define DEFAULT_AUTOSAVE_FILENAME "ATOSAVE_demo.sbm"

#ifndef AUTOSAVE_SECONDS
#define AUTOSAVE_SECONDS 10
#endif /* AUTOSAVE_SECONDS */

struct save_thread_args
{
	struct book_node *head;
	char *filename;
	pthread_mutex_t *book_mutex;
};

void start_autosave(pthread_t *thread, struct save_thread_args *args);
void stop_autosave(pthread_t *thread, struct save_thread_args *args);

#endif /* ifndef _AUTOSAVE_H_ */
