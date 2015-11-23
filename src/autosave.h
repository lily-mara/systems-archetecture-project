#ifndef _AUTOSAVE_H_
#define _AUTOSAVE_H_

#define DEFAULT_AUTOSAVE_FILENAME "ATOSAVE_demo.sbm"

#ifndef AUTOSAVE_SECONDS
#define AUTOSAVE_SECONDS 10
#endif /* AUTOSAVE_SECONDS */

void start_autosave(pthread_t *, struct book_node *);
void stop_autosave(pthread_t *);

#endif /* ifndef _AUTOSAVE_H_ */
