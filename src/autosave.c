/**CFile***********************************************************************

  FileName    autosave.c

  Synopsis    logic for autosaving parts of book manager application

  SeeAlso     autosave.h

  Copyright   [Copyright (c) 2015 Carlos III University of Madrid
  All rights reserved.

  Permission is hereby granted, without written agreement and without license
  or royalty fees, to use, copy, modify, and distribute this software and its
  documentation for any purpose, provided that the above copyright notice and
  the following two paragraphs appear in all copies of this software.

  IN NO EVENT SHALL THE CARLOS III UNIVERSITY OF MADRID BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE CARLOS III
  UNIVERSITY OF MADRID HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  THE CARLOS III UNIVERSITY OF MADRID SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN
  "AS IS" BASIS, AND CARLOS III UNIVERSITY OF MADRID HAS NO OBLIGATION TO
  PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.]

******************************************************************************/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "book.h"
#include "autosave.h"

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/

static void *save_thread_entrypoint(void *);

/*---------------------------------------------------------------------------*/
/* Definition of functions                                                   */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis           Start a new thread that will autosave the list every
                     AUTOSAVE_SECONDS seconds.

  Parameters         A pthread_t to store information about the new running
                     thread

  SideEffects        Start background thread that saves the list passed as a
                     part of the struct save_thread_args every AUTOSAVE_SECONDS

  SeeAlso            save_thread_entrypoint, stop_autosave

******************************************************************************/
void start_autosave(pthread_t *p, struct save_thread_args *args)
{
#ifdef DEBUG
	printf("[DEBUG] Enter autosave filename: ");
	args->filename = get_string();
	if (args->filename == NULL)
	{
		printf("[DEBUG] Got CTRL-D, returning to menu.\n");
		return;
	}
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


/**Function********************************************************************

  Synopsis           Stops and frees all used memory for the autosave thread.

  SideEffects        Autosave thread is killed, freed and the filename
                     associated with the autosave thread is freed.

  SeeAlso            start_autosave

******************************************************************************/
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

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis           Entrypoint for the background autosave thread

  SideEffects        Save book list every AUTOSAVE_SECONDS seconds.

  SeeAlso            start_autosave

******************************************************************************/
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
