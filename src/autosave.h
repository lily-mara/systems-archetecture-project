/**CHeaderFile*****************************************************************

  FileName    autosave.h

  Synopsis    public definitions for autosave logic

  SeeAlso     autosave.c

  Copyright   [Copyright (c) 2015 Carlos III University of Madrid
  All rights reserved.

  Permission is hereby granted, without written agreement and without license
  or royalty fees, to use, copy, modify, and distribute this software and its
  documentation for any purpose, provided that the above copyright notice and
  the following two paragraphs appear in all copies of this software.

  IN NO EVENT SHALL THE CARLOS III UNIVERSITY OF MADRID BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF CARLOS III
  UNIVERSITY OF MADRID HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  CARLOS III UNIVERSITY OF MADRID SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN
  "AS IS" BASIS, AND CARLOS III UNIVERSITY OF MADRID HAS NO OBLIGATION TO
  PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.]

******************************************************************************/
#ifndef _AUTOSAVE_H_
#define _AUTOSAVE_H_

#include <pthread.h>
#include "book.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/
#define DEFAULT_AUTOSAVE_FILENAME "ATOSAVE_demo.sbm"

#ifndef AUTOSAVE_SECONDS
#define AUTOSAVE_SECONDS 10
#endif /* AUTOSAVE_SECONDS */

/*---------------------------------------------------------------------------*/
/* Structure declarations                                                    */
/*---------------------------------------------------------------------------*/
/**Struct**********************************************************************

  Synopsis    Arguments to be passed into autosave thread.

******************************************************************************/
struct save_thread_args
{
	struct book_node *head;
	char *filename;
	pthread_mutex_t *book_mutex;
};

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void start_autosave(pthread_t *thread, struct save_thread_args *args);
void stop_autosave(pthread_t *thread, struct save_thread_args *args);

#endif /* ifndef _AUTOSAVE_H_ */
