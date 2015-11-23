#ifndef _MULTIPROC_H_
#define _MULTIPROC_H_

void start_autosave(pthread_t *, struct book *);
void stop_autosave(pthread_t *);

#endif /* ifndef _MULTIPROC_H_ */
