#ifndef _MY_BOOK_MANAGER_H_
#define _MY_BOOK_MANAGER_H_

struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	int i_numb_pages;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
	struct book *next;
};

//Prototypes
// wewertwd
#endif /* ifndef _MY_BOOK_MANAGER_H_ */
