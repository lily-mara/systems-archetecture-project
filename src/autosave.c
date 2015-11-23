#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "book.h"
#include "autosave.h"

static void *save_thread_entrypoint(void *);

void start_autosave(pthread_t *p, struct book *books)
{
	int status = pthread_create(p, NULL, save_thread_entrypoint, books);
	if (status)
	{
		printf("\t\tCould not acquire thread. Autosave failed.\n");
		return;
	}
}

static void *save_thread_entrypoint(void *b)
{
	struct book *books = (struct book *)b;
#ifdef DEBUG
	int status;
#endif /* DEBUG */
	int old_cancel_state;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
#ifdef DEBUG
	status = export_books(books, AUTOSAVE_PATH);
	printf("[DEBUG] Autosaved. Export function returned: %d\n", status);
#else /* DEBUG */
	export_books(books, AUTOSAVE_PATH);
#endif /* DEBUG */
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state);

	while(1)
	{
		sleep(AUTOSAVE_SECONDS);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
#ifdef DEBUG
		status = export_books(books, AUTOSAVE_PATH);
		printf("[DEBUG] Autosaved. Export function returned: %d\n", status);
#else /* DEBUG */
		export_books(books, AUTOSAVE_PATH);
#endif /* DEBUG */
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state);
	}

	pthread_exit(NULL);
}

void stop_autosave(pthread_t *p)
{
	int x, *y;
	y = &x;

	pthread_cancel(*p);
	pthread_join(*p, (void **) &y);
	free(p);
}
