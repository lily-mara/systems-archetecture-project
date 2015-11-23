#ifndef _MULTIPROC_H_
#define _MULTIPROC_H_

#ifndef AUTOSAVE_PATH
#define AUTOSAVE_PATH "autosave.dat"
#endif /* AUTOSAVE_PATH */

#ifndef AUTOSAVE_SECONDS
#define AUTOSAVE_SECONDS 10
#endif /* AUTOSAVE_SECONDS */

void start_autosave(pthread_t *, struct book *);
void stop_autosave(pthread_t *);

#endif /* ifndef _MULTIPROC_H_ */
