#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "book.h"
#include "autosave.h"

static void *save_thread_entrypoint(void *);

void start_autosave(pthread_t *p, struct save_thread_args *args)
{
#ifdef DEBUG
	printf("[DEBUG] Enter autosave filename: ");
	args->filename = get_string();
#else /* DEBUG */
	strcpy(args->filename, DEFAULT_AUTOSAVE_FILENAME);
#endif /* DEBUG */

	int status = pthread_create(p, NULL, save_thread_entrypoint, (void *)args);
	if (status)
	{
		printf("\t\tCould not acquire thread. Autosave failed.\n");
		return;
	}
}

static void *save_thread_entrypoint(void *b)
{
	struct save_thread_args *args = (struct save_thread_args *)b;
	struct book_node *books = args->head;
#ifdef DEBUG
	int status;
#endif /* DEBUG */
	int old_cancel_state;

	for (;;)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
#ifdef DEBUG
		pthread_mutex_lock(args->book_mutex);
		status = export_books(books, args->filename);
		pthread_mutex_unlock(args->book_mutex);
		printf("[DEBUG] Autosaved. Export function returned: %d\n", status);
#else /* DEBUG */
		pthread_mutex_lock(args->book_mutex);
		export_books(books, args->filename);
		pthread_mutex_unlock(args->book_mutex);
#endif /* DEBUG */
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state);
		sleep(AUTOSAVE_SECONDS);
	}

	pthread_exit(NULL);
}

void stop_autosave(pthread_t *p, struct save_thread_args *args)
{
	int x, *y;
	y = &x;

	if (p != NULL)
	{
		pthread_cancel(*p);
		pthread_join(*p, (void **) &y);
		free(p);
	}

	if (args != NULL)
	{
		if (args->filename != NULL)
		{
			free(args->filename);
			args->filename = NULL;
		}
	}
}
